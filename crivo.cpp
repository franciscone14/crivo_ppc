/**
 * @file crivo.cpp
 *
 * @brief O algoritmo abaixo implementa o crivo de Eratosthenes usando threads
 *
 *
 * @authors: Franciscone L. e Felipe Oliveira
 *
*/

#include <iostream>
#include <pthread.h>
#include <cmath>
#include <cstdlib>

using namespace std;

// Estrutura tipo numero que possui como atributos o numero e se esse foi marcado
typedef struct {
    int numero;
    bool marcado = false;
} Numero;

/* 
    Estrutura usando para passagem de argumentos para a thread
    A estrutura recebe uma copia do valor de K, a quantidade de numeros a ser processada
    o numero de threads um ponteiro de inicio para a posição do array onde a thread
    deve começar o processamento
*/
typedef struct {
    int k;
    int *size;
    const int *numThreads;
    Numero *inicio;
    int id;
} Arg;

// função que verifica se um numero é multiplo
bool isMultiplo(int numero, int k){
    if(numero % k == 0)
        return true;
    return false;
}

// Declaração geral da thread para processar as partes do array
void *thread(void *argv){
    // Casting do argumento recebido para o tipo Arg
    Arg *arg = (Arg*) argv;
    // Cria um ponteiro para o numero de inicio do processamento
    Numero *atual = arg->inicio;

    // Limite que indica até que ponto do array deve ser o processamento
    int fim = *(arg->size) / *(arg->numThreads);

    int k = arg->k;
    // percorre o array da posição passada (inicio) até o valor limite (fim)
    for (int i = 0; i < fim; i++){
        // Se o numero não foi marcado & se for multiplo de K & menor que K² marque o numero
        if((!atual->marcado) && isMultiplo(atual->numero, k) && ((k * k) <= atual->numero)){
            atual->marcado = true;
        }
        // Incrementa o valor do ponteiro
        atual++;
    }
    // Chamada de finalização da thread
    pthread_exit(NULL);
}

int main(){
    int n;
    int k = 2;

    cin >> n;

    // Declara a list de numeros
    Numero *list = new Numero[n];
    
    // Ler a lista de numero
    for(int i = 0; i < n; i++){
        cin >> list[i].numero;
    }

    // Define a constante do numero de threads
    const int NUM_THREADS = sqrt(float(n));
    // Array de threads
    pthread_t threads[NUM_THREADS];

    // Seguindo o principio do algoritmo do crivo, equanto k² > n não para
    while((k * k) <= n){
        // Loop para criar cada thread que vai fazer o processamento
        for(int i = 0; i < NUM_THREADS; i++){
            // Declaração dos argumentos da thread
            Arg *a = new Arg;
            // Atribuição dos valores e ponteiros
            a->k = k;
            a->size = &n;
            // Com base na thread atual e no valor de n calcula o ponteiro de inicio do array
            a->inicio = list + (i * (n / NUM_THREADS)); 
            a->numThreads = &NUM_THREADS;
            a->id = i;

            // Cria a thread passando os argumentos e verifica por erros
            int rc = pthread_create(&threads[i], NULL, thread, a);
            if (rc){
                cout << "ERROR; return code from pthread_create() is " << rc << endl;
                exit(-1);
            }
        }

        // Procura o menor numero não marcado
        bool naoAchou = true;
        for(int i = 0; i < n && naoAchou; i++){
            if((!list[i].marcado) && list[i].numero > k){
                naoAchou = false;
                k = list[i].numero;
            }
        }
    }

    // Escreve na saida padrão os numeros não marcados que são primos
    for(int i = 0; i < n; i++){
        if(!list[i].marcado){
            cout << list[i].numero << " ";
        }
    }
    cout << endl;
}
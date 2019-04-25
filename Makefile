all: main

main:
	g++ -pthread -Wall crivo.cpp -o crivo.o

run: main
	./crivo.o

clean:
	rm -rf *.o
all: cliente servidor

servidor: servidor.c sockutil.o
	gcc -o servidor servidor.c sockutil.o
cliente: cliente.c sockutil.o 
	gcc -o cliente cliente.c sockutil.o
sockutil.o: 
	gcc -c sockutil.c

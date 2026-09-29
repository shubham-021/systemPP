all: server client

server: server.o error_functions.o
	clang server.o error_functions.o -o server

server.o: server.c
	clang -c server.c -o server.o

client: client.o error_functions.o
	clang client.o error_functions.o -o client

client.o: client.c
	clang -c client.c -o client.o

error_functions.o: ../../../lib/error_functions.c
	clang -c ../../../lib/error_functions.c -o error_functions.o

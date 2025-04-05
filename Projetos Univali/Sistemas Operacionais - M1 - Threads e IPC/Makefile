all: servidor cliente

servidor: servidor.c banco.c
	gcc servidor.c banco.c -o servidor -lpthread

cliente: cliente.c
	gcc cliente.c -o cliente

clean:
	rm -f servidor cliente meu_pipe

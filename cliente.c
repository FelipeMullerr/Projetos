#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // Para o close(pipe)
#include <unistd.h>
#include "banco.h"

int main() {
    char comando[256];

    while (1) {
        printf("Digite um comando (INSERT, DELETE, SELECT ou sair):\n");
        printf("\bInsert: INSERT id=numero nome='nome' \n");
        printf("\bDelete: DELETE id=numero\n");
        printf("\bSelect: SELECT id=numero\n>");
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            break;
        }

        comando[strcspn(comando, "\n")] = '\0'; //Retirando o \n de quando é dado "Enter" da String

        int pipe_fd = open(PIPE_NOME, O_WRONLY); // se pipe_fd (file descriptor) é igual a -1, erro ao abrir o pipe
        if (pipe_fd == -1) {                     // descricao do arquivo pipe (para o programa encontrar o arquivo e conseguir abrir)
            printf("Erro ao abrir o pipe nomeado");
            continue;
        }
        // Insere o comando no Pipe nomeado para fazer a comunicao com o servidor
        write(pipe_fd, comando, strlen(comando));
        close(pipe_fd);
        
        if (strcmp(comando, "sair") == 0) {
            printf("Saindo e terminando a execução do cliente\n");
            break;
        }
    }

    return 0;
}

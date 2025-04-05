#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "banco.h"
#include <errno.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* processar_requisicao(void* arg) {
    system("clear");
    char* comando = (char*)arg;
    int id;
    char nome[TAM_NOME];

    pthread_mutex_lock(&mutex);

    if (sscanf(comando, "INSERT id=%d nome='%49[^']'", &id, nome) == 2) {
        printf("Processando: %s\n", comando);
        if (inserir_registro(id, nome)) {
            printf("Registro inserido com sucesso.\n");
        } else {
            printf("Erro ao inserir (banco cheio).\n");
        }
    } else if (sscanf(comando, "DELETE id=%d", &id) == 1) {
        printf("Processando: %s\n", comando);
        if (deletar_registro(id)) {
            printf("Registro removido com sucesso.\n");
        } else {
            printf("Registro não encontrado.\n");
        }
    } else if (sscanf(comando,"SELECT id=%d",&id) == 1) {
        selecionar_registro(id);
    } else {
        printf("Comando inválido: %s\n", comando);
    }
    if(!(sscanf(comando,"SELECT id=%d",&id) ==1)) { //Se for um SELECT nao exibe o Banco novamente
        imprimir_banco();
    }

    pthread_mutex_unlock(&mutex);

    free(comando);

    return NULL;
}

int main() {
    int pipe_fd;
    char buffer[256];
    inicializar_banco();

    if(mkfifo(PIPE_NOME, 0666)== -1) {
        if(errno != EEXIST) {
            printf("Erro ao criar o Pipe Nomeado");
            return 1;
        }
        printf("Pipe Nomeado ja existe\n");
     }
    printf("Servidor aguardando requisições...\n");


    while(1) {
        // Abrindo o pipe para ler o comando
        pipe_fd = open(PIPE_NOME,O_RDONLY);
        if(pipe_fd == -1) {
            printf("Erro ao abrir o Pipe para leitura");
            return 1;
        }
        //Lendo o Pipe para extrair o comando
        ssize_t bytes_lidos = read(pipe_fd,buffer,sizeof(buffer)-1);
        close(pipe_fd);

        // Verificando se a string recebido é maior que 0 ou seja, tem alguma string
        if(bytes_lidos > 0) {
            buffer[bytes_lidos] = '\0'; // Garante que nao irá restar nenhum lixo de memoria no Buffer caso a instrucao seja menor
            printf("Comando recebido: %s\n",buffer);
        } else {
            printf("Erro na leitura do Pipe");
            return 0;
        }

        if (strcmp(buffer, "sair") == 0) {
            printf("Saindo e terminando a execução do servidor\n");
            break;
        }

        char* comando = strdup(buffer);

        pthread_t thread;
        pthread_create(&thread,NULL,processar_requisicao,comando);
        //pthread_join(thread,NULL);
    }
    close(pipe_fd);
    return 0;
}

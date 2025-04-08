#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "banco.h"

#define MAX_REQUISICOES 5
#define NUM_THREADS 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Criacao da Fila circular para armazenar os comandos passados
char* fila[MAX_REQUISICOES];
int inicio = 0;
int fim = 0;
int tamanho = 0;
pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_requisicao = PTHREAD_COND_INITIALIZER;

void adicionar_na_fila(char* comando) {
    pthread_mutex_lock(&mutex_fila);
    while (tamanho == MAX_REQUISICOES) {
        pthread_cond_wait(&cond_requisicao, &mutex_fila);
    }
    fila[fim] = comando;
    fim = (fim + 1) % MAX_REQUISICOES;
    tamanho++;
    pthread_cond_signal(&cond_requisicao);
    pthread_mutex_unlock(&mutex_fila);
}

char* remover_da_fila() {
    pthread_mutex_lock(&mutex_fila);
    while (tamanho == 0) {
        pthread_cond_wait(&cond_requisicao, &mutex_fila);
    }
    char* comando = fila[inicio];
    inicio = (inicio + 1) % MAX_REQUISICOES;
    tamanho--;
    pthread_cond_signal(&cond_requisicao);
    pthread_mutex_unlock(&mutex_fila);
    return comando;
}
// Termino da criacao da Fila


void* processar_requisicao(void* arg) {
    int id_thread = *(int*)arg;

    while (1) {
        char* comando = remover_da_fila();
        int id;
        char nome[TAM_NOME];

        pthread_mutex_lock(&mutex);
        system("clear");
        printf("[Thread %d] Processando: %s\n", id_thread, comando);

        if (sscanf(comando, "INSERT id=%d nome='%49[^']'", &id, nome) == 2) {
            int resultado = inserir_registro(id,nome);
            if (resultado == 1) {
                printf("[Thread %d] Registro inserido com sucesso.\n", id_thread);
            } else if(resultado == -1) {
                printf("[Thread %d] Erro ao inserir (ID ja existente).\n", id_thread);
            } else {
                printf("[Thread %d] Erro ao inserir (Banco Cheio)\n",id_thread);
            }
        } else if (sscanf(comando, "DELETE id=%d", &id) == 1) {
            if (deletar_registro(id)) {
                printf("[Thread %d] Registro removido com sucesso.\n", id_thread);
            } else {
                printf("[Thread %d] Registro nao encontrado.\n", id_thread);
            }
        } else if (sscanf(comando,"SELECT id=%d", &id) == 1) {
            selecionar_registro(id);
        } else {
            printf("[Thread %d] Comando invalido: %s\n", id_thread, comando);
        }

        if (!(sscanf(comando,"SELECT id=%d", &id) == 1)) {
            imprimir_banco();
        }

        pthread_mutex_unlock(&mutex);
        free(comando);
    }
    return NULL;
}

int main() {
    int pipe_fd;
    char buffer[256];
    inicializar_banco();

    //Cria o arquivo do pipe nomeado caso ainda nao exista
    if (mkfifo(PIPE_NOME, 0666) == -1 && errno != EEXIST) {
        printf("Erro ao criar o Pipe Nomeado");
        return 1;
    }

    // Criando o pool de Threads
    pthread_t threads[NUM_THREADS];
    int thread_id[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_id[i] = i + 1;
        pthread_create(&threads[i], NULL, processar_requisicao, &thread_id[i]);
    }

    printf("Servidor aguardando requisições...\n");

    while (1) {
        //Abre o pipe para ler o comando
        pipe_fd = open(PIPE_NOME, O_RDONLY);
        if (pipe_fd == -1) {
            printf("Erro ao abrir o Pipe para leitura");
            return 1;
        }

        ssize_t bytes_lidos = read(pipe_fd, buffer, sizeof(buffer) - 1);
        close(pipe_fd);

        // Verifica se a string recebida é maior que 0
        if (bytes_lidos > 0) {
            buffer[bytes_lidos] = '\0';
            //printf("Comando recebido: %s\n", buffer);

            if (strcmp(buffer, "sair") == 0) {
                printf("Saindo e terminando a execução do servidor\n");
                break;
            }

            char* comando = strdup(buffer);
            adicionar_na_fila(comando);
        } else {
            printf("Erro na leitura do Pipe\n");
            return 0;
        }
    }

    return 0;
}
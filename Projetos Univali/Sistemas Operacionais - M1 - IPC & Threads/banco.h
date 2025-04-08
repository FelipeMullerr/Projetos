#ifndef BANCO_H
#define BANCO_H

#define MAX_REGISTROS 100
#define TAM_NOME 50
#define PIPE_NOME "meu_pipe"

typedef struct {
    int id;
    char nome[TAM_NOME];
} Registro;

extern Registro registros[MAX_REGISTROS];

void inicializar_banco();
int inserir_registro(int id, const char* nome);
int deletar_registro(int id);
int selecionar_registro(int id);
void imprimir_banco();
int update_registro(int id, const char* nome);

#endif
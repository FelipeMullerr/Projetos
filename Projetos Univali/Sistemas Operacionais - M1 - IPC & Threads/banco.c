#include <stdio.h>
#include <string.h>
#include "banco.h"

Registro registros[MAX_REGISTROS];

void inicializar_banco() {
    for (int i = 0; i < MAX_REGISTROS; i++) {
        registros[i].id = -1;
        registros[i].nome[0] = '\0';
    }
}

int inserir_registro(int id, const char* nome) {
    for (int i = 0; i < MAX_REGISTROS; i++) {
        if (registros[i].id == id) {
            return -1;
        }
    }

    for (int i = 0; i < MAX_REGISTROS; i++) {
        if (registros[i].id == -1) {
            registros[i].id = id;
            strncpy(registros[i].nome, nome, TAM_NOME);
            return 1;
        }
    }
    return 0;
}

int deletar_registro(int id) {
    for (int i = 0; i < MAX_REGISTROS; i++) {
        if (registros[i].id == id) {
            registros[i].id = -1;
            registros[i].nome[0] = '\0';
            return 1;
        }
    }
    return 0;
}

int update_registro(int id, const char* nome) {
    for (int i = 0; i < MAX_REGISTROS; i++) {
        if (registros[i].id == id) {
            strncpy(registros[i].nome, nome, TAM_NOME);
            printf("Usuario de ID %d atualizado para %s",id,nome);
            return 1;
        }
    }
    return 0;
}

int selecionar_registro(int id) {
    for(int i = 0; i < MAX_REGISTROS;i++) {
        if(registros[i].id == id) {
            printf("Usuário de ID %d encontrado com nome: %s\n",id,registros[i].nome);
            return 1;
        } else {
            printf("Usuário nao encontrado no banco de dados\n");
            return 0;
        }
    }
    return 0;
}

void imprimir_banco() {
    printf("Estado atual do banco:\n");
    for (int i = 0; i < MAX_REGISTROS; i++) {
        if (registros[i].id != -1) {
            printf("ID: %d | Nome: %s\n", registros[i].id, registros[i].nome);
        }
    }
    printf("------------------------\n");
}
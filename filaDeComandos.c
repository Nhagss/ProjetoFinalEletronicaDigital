
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_COMMANDS 100

// Definição da estrutura do comando
typedef struct Comando {
    char descricao[256];
    struct Comando *prox;
} Comando;

// Definição da fila de comandos
typedef struct {
    Comando *frente;
    Comando *tras;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Fila;

// Pool de comandos pré-alocados
Comando pool[MAX_COMMANDS];
int pool_index = 0;

// Inicializa a fila
void inicializaFila(Fila *fila) {
    fila->frente = fila->tras = NULL;
    pthread_mutex_init(&fila->mutex, NULL);
    pthread_cond_init(&fila->cond, NULL);
}

// Enfileira um comando
void enfileirar(Fila *fila, const char *descricao) {
    pthread_mutex_lock(&fila->mutex);

    if (pool_index >= MAX_COMMANDS) {
        printf("Erro: Pool de comandos cheio!\n");
        pthread_mutex_unlock(&fila->mutex);
        return;
    }

    Comando *novo = &pool[pool_index++];
    strcpy(novo->descricao, descricao);
    novo->prox = NULL;

    if (fila->tras == NULL) {
        fila->frente = fila->tras = novo;
    } else {
        fila->tras->prox = novo;
        fila->tras = novo;
    }
    pthread_cond_signal(&fila->cond);
    pthread_mutex_unlock(&fila->mutex);
}

// Desenfileira um comando
void *processarFila(void *arg) {
    Fila *fila = (Fila *)arg;
    while (1) {
        pthread_mutex_lock(&fila->mutex);
        while (fila->frente == NULL) {
            pthread_cond_wait(&fila->cond, &fila->mutex);
        }

        Comando *temp = fila->frente;
        printf("Executando: %s\n", temp->descricao);
        fila->frente = fila->frente->prox;
        if (fila->frente == NULL) {
            fila->tras = NULL;
        }

        pthread_mutex_unlock(&fila->mutex);
    }
    return NULL;
}
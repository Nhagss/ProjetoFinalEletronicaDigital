#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

// Estrutura para armazenar comandos
typedef struct Comando {
    char sql[MAX_LEN];
    struct Comando *prox;
} Comando;

// Estrutura da fila
typedef struct {
    Comando *frente, *tras;
} FilaComandos;

// Inicializa a fila
void inicializar_fila(FilaComandos *fila) {
    fila->frente = fila->tras = NULL;
}

// Insere um comando na fila
void enfileirar(FilaComandos *fila, char *sql) {
    Comando *novo = (Comando *)malloc(sizeof(Comando));
    if (!novo) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    strcpy(novo->sql, sql);
    novo->prox = NULL;

    if (!fila->frente) { // Se a fila estiver vazia
        fila->frente = fila->tras = novo;
    } else {
        fila->tras->prox = novo;
        fila->tras = novo;
    }
}

// Remove um comando da fila
Comando *desenfileirar(FilaComandos *fila) {
    if (!fila->frente) return NULL; // Fila vazia

    Comando *removido = fila->frente;
    fila->frente = fila->frente->prox;
    if (!fila->frente) fila->tras = NULL; // Se ficar vazia, ajustar tras
    return removido;
}

// Verifica se um comando é válido
int validar_comando(char *sql) {
    char copia[MAX_LEN];
    strcpy(copia, sql);
    for (int i = 0; copia[i]; i++) copia[i] = tolower(copia[i]); // Convertendo para minúsculas

    if (strstr(copia, "insert into ") == copia ||
        strstr(copia, "update ") == copia ||
        strstr(copia, "delete from ") == copia ||
        strstr(copia, "select * from ") == copia) {
        return 1; // Comando válido
    }
    return 0; // Comando inválido
}

// Processa a fila e remove comandos inválidos
void processar_fila(FilaComandos *fila) {
    FilaComandos fila_valida;
    inicializar_fila(&fila_valida);

    Comando *cmd;
    while ((cmd = desenfileirar(fila)) != NULL) {
        if (validar_comando(cmd->sql)) {
            enfileirar(&fila_valida, cmd->sql);
        } else {
            printf("Comando inválido descartado: %s\n", cmd->sql);
        }
        free(cmd);
    }

    // Transferir comandos válidos de volta para a fila original
    *fila = fila_valida;
}

// Imprime a fila
void imprimir_fila(FilaComandos *fila) {
    Comando *atual = fila->frente;
    printf("\nComandos na fila:\n");
    while (atual) {
        printf("%s\n", atual->sql);
        atual = atual->prox;
    }
}

// Libera memória da fila
void liberar_fila(FilaComandos *fila) {
    Comando *atual = fila->frente;
    while (atual) {
        Comando *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    fila->frente = fila->tras = NULL;
}


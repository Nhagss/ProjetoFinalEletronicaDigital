#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 10
#define MAX_LEN 256

// Estrutura para armazenar um comando SQL
typedef struct {
    char method[10];           // Método: insert, update, delete, search
    char keys[MAX_KEYS][MAX_LEN];  // Campos da tabela
    char values[MAX_KEYS][MAX_LEN]; // Valores correspondentes
    char extra[MAX_LEN];        // Parte extra do comando (ORDER BY, WHERE, etc.)
    int key_count;              // Quantidade de chaves
} Command;

// Função para inicializar um comando
void inicializar_comando(Command *cmd) {
    cmd->method[0] = '\0';
    cmd->extra[0] = '\0';
    cmd->key_count = 0;
}

// Função para remover espaços extras
void trim(char *str) {
    int i, start = 0, end = strlen(str) - 1;
    while (str[start] == ' ') start++;
    while (end > start && (str[end] == ' ' || str[end] == ';')) end--;
    for (i = 0; start <= end; i++, start++) str[i] = str[start];
    str[i] = '\0';
}

// Função para extrair os valores entre parênteses
void extrair_entre_parenteses(const char *str, char *dest) {
    const char *inicio = strchr(str, '(');
    const char *fim = strchr(str, ')');
    if (inicio && fim && fim > inicio) {
        strncpy(dest, inicio + 1, fim - inicio - 1);
        dest[fim - inicio - 1] = '\0';
    } else {
        dest[0] = '\0';
    }
}

// Função para dividir string por vírgula e armazenar em um array
int dividir_por_virgula(const char *str, char arr[MAX_KEYS][MAX_LEN]) {
    char temp[MAX_LEN];
    strcpy(temp, str);
    int count = 0;
    char *token = strtok(temp, ",");
    while (token && count < MAX_KEYS) {
        trim(token);
        strcpy(arr[count], token);
        count++;
        token = strtok(NULL, ",");
    }
    return count;
}

// Função para analisar um comando SQL e preenchê-lo na struct Command
int analisar_comando(char *sql, Command *cmd) {
    inicializar_comando(cmd);
    trim(sql);

    if (strncmp(sql, "insert into ", 12) == 0) {
        strcpy(cmd->method, "insert");
        char *values_ptr = strstr(sql, "values");
        if (!values_ptr) return 0; // Comando inválido

        *values_ptr = '\0'; // Separa colunas e valores
        values_ptr += 6;

        char colunas[MAX_LEN], valores[MAX_LEN];
        extrair_entre_parenteses(sql + 12, colunas);
        extrair_entre_parenteses(values_ptr, valores);

        cmd->key_count = dividir_por_virgula(colunas, cmd->keys);
        dividir_por_virgula(valores, cmd->values);
    }
    else if (strncmp(sql, "update ", 7) == 0) {
        strcpy(cmd->method, "update");
        char *set_ptr = strstr(sql, " set ");
        char *where_ptr = strstr(sql, " where ");
        if (!set_ptr) return 0; // Comando inválido

        *set_ptr = '\0';
        set_ptr += 5;

        if (where_ptr) {
            *where_ptr = '\0';
            where_ptr += 7;
            strcpy(cmd->extra, where_ptr);
        }

        char colunas_valores[MAX_KEYS][MAX_LEN];
        int count = dividir_por_virgula(set_ptr, colunas_valores);
        for (int i = 0; i < count; i++) {
            char *igual = strchr(colunas_valores[i], '=');
            if (!igual) return 0; // Formato inválido
            *igual = '\0';
            trim(colunas_valores[i]);
            trim(igual + 1);
            strcpy(cmd->keys[i], colunas_valores[i]);
            strcpy(cmd->values[i], igual + 1);
        }
        cmd->key_count = count;
    }
    else if (strncmp(sql, "delete from ", 12) == 0) {
        strcpy(cmd->method, "delete");
        char *where_ptr = strstr(sql, " where ");
        if (where_ptr) {
            strcpy(cmd->extra, where_ptr + 7);
        }
    }
    else if (strncmp(sql, "select * from ", 14) == 0) {
        strcpy(cmd->method, "search");
        strcpy(cmd->keys[0], "*");
        strcpy(cmd->values[0], "*");

        char *extra_ptr = strstr(sql, " order by ");
        if (extra_ptr) {
            strcpy(cmd->extra, extra_ptr);
        }
    } else {
        return 0; // Comando inválido
    }
    return 1; // Comando válido
}

// Função para imprimir um comando formatado
void imprimir_comando(const Command *cmd) {
    printf("\nMétodo: %s\n", cmd->method);
    printf("Keys: [ ");
    for (int i = 0; i < cmd->key_count; i++) {
        printf("'%s' ", cmd->keys[i]);
    }
    printf("]\n");

    printf("Values: [ ");
    for (int i = 0; i < cmd->key_count; i++) {
        printf("'%s' ", cmd->values[i]);
    }
    printf("]\n");

    if (strlen(cmd->extra) > 0) {
        printf("Extra: '%s'\n", cmd->extra);
    }
}


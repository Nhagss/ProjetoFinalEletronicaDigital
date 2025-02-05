# Gerenciamento de Pets e Pessoas em C

## 📌 Sobre o Projeto

Este projeto implementa um sistema de gerenciamento de pessoas e seus pets utilizando **listas duplamente encadeadas** para armazenamento dos dados e **filas** para processamento de comandos estruturados no formato **SQL-like**. Além disso, o sistema permite ordenação de consultas através de **árvores binárias**.

## 🛠️ Funcionalidades Principais

- **Gerenciamento de Pessoas**: Cadastro, edição, remoção e listagem de pessoas.
- **Gerenciamento de Tipos de Pets**: Cadastro e listagem de tipos de pets.
- **Gerenciamento de Pets**: Cadastro, edição, remoção e listagem de pets vinculados a pessoas.
- **Processamento de Comandos SQL-like**:
  - `INSERT`, `UPDATE`, `DELETE` e `SELECT` com `ORDER BY`.
  - Separação dos comandos em filas específicas (pessoas, tipos de pets e pets).
  - Validação de comandos antes da execução.
- **Persistência de Dados**: Os dados são armazenados e recuperados de arquivos.
- **Ordenação de Resultados**: `SELECT * FROM pet ORDER BY nome` é processado através de uma **árvore binária**.

## 🏗️ Estrutura do Projeto

O código é modularizado em arquivos `.c` e `.h`:

- `pessoa.c` e `pessoa.h` → Gerenciamento de pessoas.
- `tipo_pet.c` e `tipo_pet.h` → Gerenciamento de tipos de pets.
- `pet.c` e `pet.h` → Gerenciamento de pets.
- `fila.c` e `fila.h` → Implementação das filas de comandos.
- `arvore.c` e `arvore.h` → Implementação da árvore binária.
- `main.c` → Arquivo principal que gerencia a execução do sistema.

## 🔗 Regras de Negócio

- Códigos (IDs) de **pessoas, pets e tipos de pets são únicos**.
- Um **pet deve pertencer a uma pessoa válida** e seu tipo deve ser um tipo de pet cadastrado.
- Uma **pessoa com pets não pode ser removida** sem antes excluir seus pets.
- As operações de **alteração** de pets garantem a integridade dos IDs de dono e tipo.
- **Comandos inválidos são descartados** antes da execução.

## 🚧 Desafios Enfrentados

Durante o desenvolvimento deste projeto, enfrentamos diversos desafios, incluindo:
- **Gerenciamento eficiente de memória** ao trabalhar com listas duplamente encadeadas e filas dinâmicas.
- **Validação de comandos SQL-like**, garantindo que apenas operações bem formatadas sejam processadas.
- **Manutenção da consistência dos dados** entre arquivos e estruturas dinâmicas.
- **Implementação da árvore binária** para ordenação dos resultados de consultas.
- **Criação de um sistema modular** e bem estruturado para facilitar futuras manutenções.

## 🏁 Executando o Projeto

### 🔧 Compilação

Para compilar o programa, utilize um compilador GCC e rode o seguinte comando no terminal:

```sh
gcc -o sistema main.c pessoa.c tipo_pet.c pet.c fila.c arvore.c -Wall
```

### ▶️ Execução

Após a compilação, execute o programa:

```sh
./sistema
```

O programa permitirá a inserção de comandos diretamente pelo usuário ou a leitura de um **arquivo de script**.

## 📄 Exemplo de Script SQL-like

```sql
insert into tipo_pet(codigo, descricao) values(1, 'cachorro');
insert into pessoa(codigo, nome, fone) values(1, 'João', '859995566');
insert into pet(codigo, codigo_cli, nome, codigo_tipo) values(1,1, 'Bilu', 1);
select * from pet order by nome;
delete from pet where codigo = 1;
```

## 🏗️ Tecnologias Utilizadas

- **Linguagem C**
- **Estruturas de Dados Dinâmicas** (Listas Duplamente Encadeadas, Filas e Árvores Binárias)
- **Manipulação de Arquivos**

## ✨ Autores

Desenvolvido como parte de um trabalho acadêmico para a disciplina de **Estrutura de Dados**.

- [Nhagss](https://github.com/Nhagss)
- [João Roberto](https://github.com/joaorbrto)

Desenvolvido como parte de um trabalho acadêmico para a disciplina de **Estrutura de Dados**.

📌 **Status:** Em desenvolvimento 🚧


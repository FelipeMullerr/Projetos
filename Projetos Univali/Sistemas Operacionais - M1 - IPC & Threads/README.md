# Sistema de Gerenciamento de Registros com Comunicação via Pipe

Este projeto foi desenvolvido como parte da disciplina de **Sistemas Operacionais**.

## 📌 Descrição Geral

O sistema simula o funcionamento interno de um gerenciador de requisições a um banco de dados, utilizando múltiplos processos e threads. A proposta é aplicar conceitos fundamentais de sistemas operacionais como comunicação entre processos, concorrência e sincronização.

### 🛠️ Estrutura do Sistema

- Um processo **cliente** envia requisições de consulta, inserção, remoção ou atualização via IPC (utilizando **pipes nomeados**);
- Um processo **servidor** recebe as requisições e utiliza múltiplas **threads** para processá-las em paralelo;
- As threads utilizam **mutexes** e **variáveis de condição** para garantir o acesso seguro a uma estrutura de dados compartilhada (um vetor de registros em memória que simula um banco de dados).

---

## 🔧 Visão do Projeto

O sistema consiste em dois programas principais:

- **Servidor:** Responsável por processar os comandos enviados, gerenciar os registros em memória e registrar logs.
- **Cliente:** Interface simples via terminal para o envio de comandos ao servidor.

A comunicação entre cliente e servidor ocorre via **pipe nomeado**. O servidor processa os comandos de forma concorrente, utilizando múltiplas threads e uma fila circular para gerenciar as requisições recebidas.

---

## 🧠 Conceitos Trabalhados

- Pipes nomeados (mkfifo)
- Threads com pthread
- Mutexes e variáveis de condição para controle de concorrência
- Fila circular para gerenciar requisições
- Comunicação cliente-servidor
- Sincronização de acesso a recursos compartilhados

---

### 🦾 Funcionalidades Suportadas

- `INSERT id=numero nome='nome'`: Insere um novo registro com `id` e `nome`.
- `DELETE id=numero`: Remove o registro com o `id` especificado.
- `SELECT id=numero`: Mostra o nome do registro com o `id` especificado.
- `UPDATE id=numero novoNome='novo_nome'`: Atualiza o nome de um registro.
- `sair`: Encerra o cliente ou o servidor, dependendo de onde for digitado.

---

## ⚙️ Requisitos

- Sistema operacional: **Linux**
- Compilador: `gcc`
- Utilização de comandos padrão do sistema (como `mkfifo` e `clear`)

---

## 🚀 Como Executar

1. Clone ou baixe o projeto para seu computador.

2. Acesse a pasta onde o projeto está localizado usando o comando `cd`.

3. Compile o projeto utilizando o comando `make all`.

4. Em seguida, abra **dois terminais**: um será usado para executar o servidor e o outro para o cliente.

5. Em cada terminal, navegue até a pasta do projeto com o comando `cd`, caso ainda não esteja nela.

6. No **primeiro terminal**, execute o servidor digitando `./servidor`.

7. No **segundo terminal**, execute o cliente digitando `./cliente`.

8. Agora, com ambos em execução, digite os comandos no terminal do cliente para interagir com o servidor. Você pode utilizar os comandos `INSERT`, `DELETE`, `SELECT`, `UPDATE` ou `sair` para encerrar o sistema.


* minishell
a small reproduction of bash

# Minishell

Minishell è un semplice shell Unix sviluppato in C, progettato per replicare funzionalità basilari di un terminale bash. Questo progetto ha l'obiettivo di offrire un ambiente di shell minimale, ma funzionante, con supporto a comandi, pipe, redirezioni, gestione delle variabili d'ambiente e segnali.

## Caratteristiche principali

- Parsing e gestione dei comandi e argomenti
- Supporto per pipeline (`|`)
- Redirezioni input/output (`<`, `>`, `>>`)
- Gestione delle variabili d'ambiente e loro espansione (`$VAR`)
- Supporto a heredoc (`<<`)
- Gestione di quote singole e doppie per parsing accurato
- Gestione dei segnali (SIGINT, SIGQUIT)
- Implementazione di un sistema di exit status
- Strutture dati modulari per tokenizzazione, comandi e pipe
- Gestione robusta della memoria e dei file descriptor

## Struttura del progetto

Il progetto è organizzato in moduli distinti che gestiscono parsing, esecuzione, espansione delle variabili, gestione di segnali e I/O. Include anche un set completo di funzioni di utilità per la manipolazione di token, comandi e variabili d’ambiente.

## Come compilare

```bash
make
--------------English version--------------------
# Minishell

Minishell is a simple Unix shell developed in C, designed to replicate the basic functionalities of a bash terminal. This project aims to provide a minimal yet functional shell environment, supporting commands, pipes, redirections, environment variable management, and signal handling.

## Main Features

- Parsing and handling commands and arguments
- Support for pipelines (`|`)
- Input/output redirections (`<`, `>`, `>>`)
- Environment variable management and expansion (`$VAR`)
- Support for heredoc (`<<`)
- Handling of single and double quotes for accurate parsing
- Signal handling (SIGINT, SIGQUIT)
- Implementation of an exit status system
- Modular data structures for tokenization, commands, and pipes
- Robust memory and file descriptor management

## Project Structure

The project is organized into distinct modules that handle parsing, execution, variable expansion, signal management, and I/O. It also includes a complete set of utility functions for manipulating tokens, commands, and environment variables.

## How to Compile

```bash
make



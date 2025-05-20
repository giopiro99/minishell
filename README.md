* minishell
a small reproduction of bash

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
make vall for track fds, leaks, ecc...

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:51 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:13:47 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Ripristina lo standard input duplicato precedentemente.
 *
 * Duplica il file descriptor salvato in `saved_stdin` sullo standard input (fd 0),
 * poi chiude il file descriptor salvato.
 *
 * @param saved_stdin Puntatore al file descriptor da ripristinare
 */
void	restore_sdin(int *saved_stdin)
{
	dup2(*saved_stdin, 0);
	close (*saved_stdin);
}

/**
 * @brief Gestisce la redirezione dell'input da un file.
 *
 * Duplica lo standard input e salva il fd duplicato in `saved_stdin`.
 * Apre il file `input_file` in sola lettura e duplica il suo fd sullo standard input.
 *
 * @param input_file Nome del file di input
 * @param saved_stdin Puntatore al file descriptor per salvare lo standard input originale
 * @return int 1 se successo, 0 se input_file è NULL, -1 in caso di errore
 */
int	handle_input_redirection(char *input_file, int	*saved_stdin)
{
	int	fd;

	fd = 0;
	if (!input_file)
		return (0);
	*saved_stdin = dup(0);
	if (*saved_stdin < 0)
		return (perror("dup"), -1);
	fd = open(input_file, O_RDONLY);
	if (fd == -1)
		return (perror(input_file), close(*saved_stdin), -1);
	if (dup2(fd, 0))
		return (close(fd), perror("dup2"), close(*saved_stdin), -1);
	return (close(fd), 1);
}

/**
 * @brief Aggiunge un elemento `pipe` alla lista concatenata di pipe.
 *
 * @param head Puntatore al puntatore della testa della lista pipe
 * @param pipe Nuovo elemento pipe da aggiungere
 */
void	add_pipe(t_pipe **head, t_pipe *pipe)
{
	t_pipe	*tmp;

	tmp = NULL;
	if (!head | !pipe)
		return ;
	if (*head == NULL)
		*head = pipe;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = pipe;
	}
}


/**
 * @brief Verifica se la lista di token contiene un token di tipo PIPE.
 *
 * @param token Puntatore alla testa della lista di token
 * @return true Se è presente un token PIPE nella lista
 * @return false Altrimenti
 */
bool	has_pipe(t_token *token)
{
	while (token)
	{
		if (token->type == PIPE)
			return (true);
		token = token->next;
	}
	return (false);
}


/**
 * @brief Imposta il flag pipe_flag a true per tutti i comandi nella lista.
 *
 * @param head Puntatore alla testa della lista di comandi
 */
void	set_pipe_flag(t_cmd *head)
{
	t_cmd	*tmp;

	tmp = head;
	while (head)
	{
		head->pipe_flag = true;
		head = head->next;
	}
	head = tmp;
}

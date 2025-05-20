/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:46:05 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:53:24 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Gestisce la redirezione dell’input da file o heredoc.
 *
 * Se il tipo è REDIR_IN, apre il file in lettura e salva lo stdin originale.
 * Se il tipo è HEREDOC, crea il file temporaneo con il contenuto heredoc e reindirizza stdin su di esso.
 * Se la redirezione fallisce, restituisce -1.
 *
 * @param tokens Token corrente che contiene il nome del file o heredoc
 * @param type Tipo di redirezione (REDIR_IN o HEREDOC)
 * @param env Ambiente della shell, necessario per heredoc
 * @param saved_stdin Puntatore per salvare il file descriptor stdin originale
 * @return int 0 se successo, -1 se errore
 */
int	ft_handle_infile_redir(t_token *tokens, t_type type,
	t_env *env, int *saved_stdin)
{
	if (type == REDIR_IN)
	{
		if (handle_input_redirection(tokens->value, saved_stdin) == -1)
			return (-1);
	}
	else if (type == HEREDOC)
	{
		handle_heredoc(tokens, env);
		if (handle_input_redirection("temp_file.txt", saved_stdin) == -1)
			return (-1);
		if (tokens->next)
		{
			if (ft_found_heredoc(tokens->next))
				ft_end_heredoc(tokens);
		}
	}
	return (0);
}


/**
 * @brief Gestisce redirezioni senza comando associato.
 *
 * Scorre i token fino a trovare FILE_ o DELIMITER e apre il file corrispondente
 * in base al tipo di redirezione (OUT, APPEND, IN, HEREDOC).
 * Per input reindirizzato, usa ft_handle_infile_redir.
 * Chiude i file descriptor aperti non necessari.
 * Se fallisce l’apertura del file, stampa errore e restituisce -1.
 *
 * @param type Tipo di redirezione (REDIR_OUT, REDIR_APPEND, REDIR_IN, HEREDOC)
 * @param tokens Lista di token da cui ottenere il file o delimiter
 * @param env Ambiente della shell
 * @return int Tipo di redirezione se successo, -1 se errore
 */
int	handle_redirection_without_command(t_type type,
		t_token *tokens, t_env *env)
{
	int	fd;
	int	saved_stdin;

	saved_stdin = 0;
	fd = 0;
	while (tokens && (tokens->type != FILE_ && tokens->type != DELIMITER))
		tokens = tokens->next;
	if (type == REDIR_OUT)
		fd = open(tokens->value, O_CREAT | O_TRUNC | O_WRONLY | O_RDONLY, 0777);
	if (type == REDIR_IN || type == HEREDOC)
	{
		if (ft_handle_infile_redir(tokens, type, env, &saved_stdin) == -1)
			return (-1);
	}
	if (type == REDIR_APPEND)
		fd = open(tokens->value, O_CREAT | O_APPEND
				| O_WRONLY | O_RDONLY, 0777);
	if (fd == -1)
		return (perror("open"), -1);
	if (fd != 1 && fd != 0)
		close(fd);
	if (type == REDIR_IN || type == HEREDOC)
		restore_sdin(&saved_stdin);
	return (type);
}

/**
 * @brief Gestisce la redirezione associata a un comando.
 *
 * Chiama handle_redirection_without_command per gestire la redirezione a basso livello.
 * Se il comando esiste, aggiorna il campo input o output nel comando in base al tipo.
 * Gestisce anche l’assegnazione del nome file o heredoc nella struttura comando.
 * Restituisce il tipo di redirezione o -1 in caso di errore.
 *
 * @param cmd Puntatore alla struttura comando (può essere NULL)
 * @param type Tipo di redirezione da gestire
 * @param tokens Lista di token con informazioni sulla redirezione
 * @param env Ambiente della shell
 * @return t_type Tipo di redirezione gestita o -1 in caso di errore
 */
t_type	handle_redirection(t_cmd *cmd, t_type type, t_token *tokens, t_env *env)
{
	t_type	redir_type;

	redir_type = handle_redirection_without_command(type, tokens, env);
	if (redir_type == (t_type)-1)
		return (-1);
	if (!cmd)
		return (redir_type);
	while (tokens && (tokens->type != FILE_ && tokens->type != DELIMITER))
		tokens = tokens->next;
	if (ft_get_outfile_name(cmd, tokens, type))
		return (0);
	else if (type == REDIR_IN)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup(tokens->value);
	}
	else if (type == HEREDOC)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup("temp_file.txt");
	}
	return (redir_type);
}

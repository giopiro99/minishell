/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:32:07 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:02:20 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>

/**
 * @brief Avanza nella lista dei token fino a raggiungere l'indice dell'argomento richiesto.
 *
 * Scorre i token fino a trovare il token corrispondente all'argomento con indice arg_index,
 * considerando i tipi ARGUMENT, ENV_VAR, QUOTE, EXIT_STATUS.
 *
 * @param token Puntatore al token di partenza
 * @param arg_index Indice dell'argomento da raggiungere
 * @return Puntatore al token trovato o NULL se non trovato
 */
static t_token	*advance_token_to_arg(t_token *token, int arg_index)
{
	int	count;

	count = -1;
	while (token)
	{
		if (token->type == ARGUMENT || token->type == ENV_VAR
			|| token->type == QUOTE || token->type == EXIT_STATUS)
			count++;
		if (count == arg_index)
			break ;
		token = token->next;
	}
	return (token);
}

/**
 * @brief Controlla la presenza dell'opzione -n nel comando echo.
 *
 * Analizza gli argomenti del comando a partire dall'indice *i,
 * verificando se sono presenti opzioni -n valide (anche multiple e concatenate),
 * e aggiorna l'indice i oltre le opzioni valide.
 *
 * @param cmd Puntatore alla struttura comando
 * @param i Puntatore all'indice degli argomenti, aggiornato all'uscita
 * @return 1 se è stata trovata almeno un'opzione -n valida, 0 altrimenti
 */
static int	ft_check_newline_flag(t_cmd *cmd, int *i)
{
	int	j;
	int	newline;

	newline = 0;
	while (cmd->args[*i])
	{
		if (cmd->args[*i][0] != '-' || cmd->args[*i][1] != 'n')
			break ;
		j = 2;
		while (cmd->args[*i][j] == 'n')
			j++;
		if (cmd->args[*i][j] != '\0')
			break ;
		newline = 1;
		(*i)++;
	}
	return (newline);
}

/**
 * @brief Scrive gli argomenti del comando echo sul file descriptor specificato.
 *
 * Gestisce gli spazi correttamente basandosi sui token corrispondenti,
 * ignorando i token di tipo GARBAGE e SPACES dove opportuno.
 *
 * @param args Array di argomenti da scrivere
 * @param fd File descriptor su cui scrivere l'output
 * @param token Puntatore al token corrente usato per determinare spaziature
 */
static void	ft_write_echo(char **args, int fd, t_token *token)
{
	int	i;

	i = 0;
	while (args[i])
	{
		write(fd, args[i], ft_strlen(args[i]));
		i++;
		token = token->next;
		while (token && token->type == GARBAGE)
			token = token->next;
		if (token && token->type == SPACES && args[i])
			write(fd, " ", 1);
		while (token && token->type == SPACES)
			token = token->next;
		while (token && token->type != ARGUMENT && token->type != QUOTE
			&& token->type != ENV_VAR && token->type != EXIT_STATUS)
			token = token->next;
	}
}

/**
 * @brief Implementa il comando builtin echo.
 *
 * Gestisce le opzioni -n multiple, stampa gli argomenti separati da spazi,
 * e termina con newline solo se non è presente l'opzione -n.
 * Gestisce input/output ridiretti e aggiorna lo stato di uscita.
 *
 * @param cmd Puntatore alla struttura comando corrente
 * @param token Puntatore alla lista di token associati al comando
 * @param env Puntatore alla struttura ambiente shell
 * @param head Puntatore alla lista di pipe, usato per liberare memoria in caso di pipe
 */
void	ft_echo(t_cmd *cmd, t_token *token, t_env *env, t_pipe *head)
{
	int	fd;
	int	saved_stdin;
	int	i;
	int	newline;

	i = 0;
	saved_stdin = 0;
	newline = 0;
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return ;
	newline = ft_check_newline_flag(cmd, &i);
	token = advance_token_to_arg(token, i);
	ft_write_echo(&cmd->args[i], fd, token);
	if (!newline)
		write(fd, "\n", 1);
	if (cmd->input)
		restore_sdin(&saved_stdin);
	if (fd != 1)
		close(fd);
	env->exit_status = 0;
	if (cmd->pipe_flag)
		free_all_pipe(head, env);
}

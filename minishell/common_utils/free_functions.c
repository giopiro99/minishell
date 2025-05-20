/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:21:14 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:21:43 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"
#include <unistd.h>

/**
 * @brief Libera tutte le risorse associate a una lista di pipe e all'ambiente.
 *
 * Libera token, comandi, ambiente, chiude pipe e file descriptor standard.
 * Rimuove il file temporaneo heredoc se presente.
 *
 * @param head_pipe puntatore alla testa della lista di pipe
 * @param env puntatore alla struttura ambiente
 */
void	free_all_pipe(t_pipe *head_pipe, t_env *env)
{
	if (head_pipe->tokens)
		free_token(head_pipe->tokens);
	if (env->env_copy)
		ft_free_matrix(env->env_copy);
	if (env)
		free(env);
	if (head_pipe->cmd)
		free_cmd(head_pipe->cmd);
	if (head_pipe->pipefd[0] != -1)
		close(head_pipe->pipefd[0]);
	if (head_pipe->pipefd[1] != -1)
		close(head_pipe->pipefd[1]);
	if (head_pipe)
		free_pipe(head_pipe);
	close (0);
	close (1);
}

/**
 * @brief Libera tutte le risorse associate a una lista di comandi e all'ambiente.
 *
 * Libera token associati ai comandi, lista di comandi e matrice ambiente.
 *
 * @param cmd puntatore alla lista di comandi
 * @param env puntatore alla struttura ambiente
 */
void	free_all(t_cmd *cmd, t_env *env)
{
	free_token(cmd->tokens);
	free_cmd(cmd);
	ft_free_matrix(env->env_copy);
	free(env);
}

/**
 * @brief Libera una lista di token.
 *
 * Per ogni token libera la stringa value e la struttura token stessa.
 *
 * @param token puntatore alla lista di token
 */
void	free_token(t_token *token)
{
	t_token	*tmp;

	tmp = NULL;
	while (token)
	{
		tmp = token->next;
		if (token->value)
			free(token->value);
		if (token)
			free(token);
		token = tmp;
	}
}

/**
 * @brief Libera una lista di comandi.
 *
 * Per ogni comando libera l'array args, le stringhe cmd, input, output,
 * rimuove il file temporaneo heredoc e libera la struttura comando.
 *
 * @param cmd_list puntatore alla lista di comandi
 */
void	free_cmd(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	tmp = NULL;
	while (cmd_list)
	{
		tmp = cmd_list->next;
		if (cmd_list->args)
			ft_free_matrix(cmd_list->args);
		if (cmd_list->cmd)
			free(cmd_list->cmd);
		if (cmd_list->input)
			free(cmd_list->input);
		if (cmd_list->output)
			free(cmd_list->output);
		unlink("temp_file.txt");
		free(cmd_list);
		cmd_list = tmp;
	}
}

/**
 * @brief Libera una lista di pipe.
 *
 * Per ogni elemento libera la struttura pipe.
 *
 * @param pipe puntatore alla lista di pipe
 */
void	free_pipe(t_pipe *pipe)
{
	t_pipe	*tmp;

	tmp = NULL;
	while (pipe)
	{
		tmp = pipe->next;
		if (pipe)
			free(pipe);
		pipe = tmp;
	}
}

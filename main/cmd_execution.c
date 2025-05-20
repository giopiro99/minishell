/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:38:37 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:09:21 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Esegue un comando all'interno di una pipeline.
 *
 * Se il comando è un built-in, lo esegue direttamente. Altrimenti,
 * invoca l'esecuzione di un comando di sistema.
 * Dopo l'esecuzione libera la memoria della pipeline.
 *
 * @param current Nodo corrente della pipeline
 * @param env Ambiente della shell
 * @param token Token associato al comando
 * @param head Inizio della pipeline (usato per la liberazione)
 */
void	ft_exe_pipe_cmd(t_pipe *current, t_env *env,
	t_token *token, t_pipe *head)
{
	if (!ft_strcmp(current->cmd->cmd, "echo"))
		ft_echo(current->cmd, token, env, head);
	else if (!ft_strcmp(current->cmd->cmd, "pwd"))
		ft_pwd(current->cmd, env, head);
	else if (!ft_strcmp(current->cmd->cmd, "cd"))
		ft_cd(current->cmd, env, head);
	else if (!ft_strcmp(current->cmd->cmd, "exit"))
		ft_exit(current->cmd, env, head);
	else if (!ft_strcmp(current->cmd->cmd, "env"))
		ft_env(current->cmd, env, head);
	else if (!ft_strcmp(current->cmd->cmd, "export"))
		ft_export(current->cmd, env, head);
	else if (!ft_strcmp(current->cmd->cmd, "unset"))
		ft_unset(current->cmd, env, head);
	else
	{
		ft_exe_syscmd_in_pipe(env, current->cmd);
		free_all_pipe(head, env);
	}
	return ;
}

/**
 * @brief Esegue una lista di comandi non pipati.
 *
 * Per ogni comando nella lista, identifica se è un built-in
 * ed esegue la funzione corrispondente. Altrimenti, esegue
 * il comando come processo esterno.
 *
 * @param cmd_list Lista dei comandi da eseguire
 * @param env Ambiente della shell
 * @param token Token associati ai comandi
 */
void	ft_exe_cmd(t_cmd *cmd_list, t_env *env, t_token *token)
{
	while (cmd_list)
	{
		if (!ft_strcmp(cmd_list->cmd, "echo"))
			ft_echo(cmd_list, token, env, NULL);
		else if (!ft_strcmp(cmd_list->cmd, "pwd"))
			ft_pwd(cmd_list, env, NULL);
		else if (!ft_strcmp(cmd_list->cmd, "cd"))
			ft_cd(cmd_list, env, NULL);
		else if (!ft_strcmp(cmd_list->cmd, "exit"))
			ft_exit(cmd_list, env, NULL);
		else if (!ft_strcmp(cmd_list->cmd, "env"))
			ft_env(cmd_list, env, NULL);
		else if (!ft_strcmp(cmd_list->cmd, "export"))
			ft_export(cmd_list, env, NULL);
		else if (!ft_strcmp(cmd_list->cmd, "unset"))
			ft_unset(cmd_list, env, NULL);
		else
			ft_exe_syscmd(env, cmd_list);
		cmd_list = cmd_list->next;
	}
}

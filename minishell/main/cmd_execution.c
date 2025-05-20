/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:38:37 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:02:47 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Executes a command within a pipeline.
 *
 * If the command is a built-in, executes it directly.
 * Otherwise, runs it as an external system command.
 * After execution, frees the memory associated with the pipeline.
 *
 * @param current Current node in the pipeline
 * @param env Shell environment
 * @param token Token associated with the command
 * @param head Head of the pipeline list (used for freeing memory)
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
 * @brief Executes a list of non-pipelined commands.
 *
 * For each command in the list, checks if it is a built-in command
 * and executes the corresponding function. Otherwise, executes
 * it as an external system command.
 *
 * @param cmd_list List of commands to execute
 * @param env Shell environment
 * @param token Tokens associated with the commands
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

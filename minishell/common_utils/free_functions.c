/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:21:14 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:56:24 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"
#include <unistd.h>

/**
 * @brief Frees all resources associated with a list of pipes and the environment.
 *
 * Frees tokens, commands, environment, closes pipes and standard file descriptors.
 * Removes the temporary heredoc file if present.
 *
 * @param head_pipe pointer to the head of the pipe list
 * @param env pointer to the environment structure
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
  * @brief Frees all resources associated with a list of commands and the environment.
  *
  * Frees tokens associated with commands, command list, and environment matrix.
  *
  * @param cmd pointer to the command list
  * @param env pointer to the environment structure
  */
 void	free_all(t_cmd *cmd, t_env *env)
 {
	 free_token(cmd->tokens);
	 free_cmd(cmd);
	 ft_free_matrix(env->env_copy);
	 free(env);
 }

 /**
  * @brief Frees a list of tokens.
  *
  * For each token, frees the string value and the token structure itself.
  *
  * @param token pointer to the token list
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
  * @brief Frees a list of commands.
  *
  * For each command, frees the args array, cmd, input, output strings,
  * removes the temporary heredoc file, and frees the command structure.
  *
  * @param cmd_list pointer to the command list
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
  * @brief Frees a list of pipes.
  *
  * For each element, frees the pipe structure.
  *
  * @param pipe pointer to the pipe list
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

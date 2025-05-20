/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:40:29 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:04:40 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"
#include <stdio.h>

/**
 * @brief Creates a linked list of pipe structures from the commands.
 *
 * Each command in the list is wrapped into a `t_pipe` node,
 * which is then linked to form a pipeline.
 *
 * @param cmd List of commands to execute
 * @param token List of associated tokens
 * @return t_pipe* Head of the created pipe list
 */
 t_pipe	*create_pipe_list(t_cmd *cmd, t_token *token)
 {
	 t_pipe	*current_pipe;
	 t_pipe	*head_pipe;

	 current_pipe = NULL;
	 head_pipe = NULL;
	 while (cmd)
	 {
		 current_pipe = create_pipe_node(cmd, token);
		 add_pipe(&head_pipe, current_pipe);
		 cmd = cmd->next;
	 }
	 return (head_pipe);
 }

 /**
  * @brief Creates a pipeline node containing a command.
  *
  * Allocates memory and initializes all necessary fields,
  * including the file descriptor array for the pipe.
  *
  * @param cmd Pointer to the command to execute
  * @param head_token List of associated tokens
  * @return t_pipe* Initialized pipeline node
  */
 t_pipe	*create_pipe_node(t_cmd *cmd, t_token *head_token)
 {
	 t_pipe	*node;

	 node = malloc(sizeof(t_pipe));
	 if (!node)
		 return (NULL);
	 node->pid = -1;
	 node->cmd = cmd;
	 node->tokens = head_token;
	 node->pipefd[0] = -1;
	 node->pipefd[1] = -1;
	 node->one_cmd_flag = false;
	 node->next = NULL;
	 return (node);
 }

 /**
  * @brief Executes an external command with `execve`, redirecting stdout if needed.
  *
  * If `fd` is different from STDOUT, performs `dup2` for redirection.
  * Then launches the binary using `execve` with the custom environment.
  * On error, prints the message and sets `exit_status` to 126.
  *
  * @param fd Pointer to the output file descriptor
  * @param dir Absolute path of the binary to execute
  * @param matrix_exe Argument matrix for execve
  * @param env Shell environment
  */
 void	ft_execution_execve(int *fd, char *dir, char **matrix_exe, t_env *env)
 {
	 if (*fd != 1)
	 {
		 if (dup2(*fd, STDOUT_FILENO) == -1)
			 return (perror("dup2"));
		 close(*fd);
	 }
	 if (dir && execve(dir, matrix_exe, env->env_copy) == -1)
	 {
		 ft_free_matrix(matrix_exe);
		 free(dir);
		 env->exit_status = 126;
		 perror("execve");
	 }
 }

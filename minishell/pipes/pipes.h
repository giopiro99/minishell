/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:54:55 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:05:40 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "../common_utils/common_utils.h"
# include "../libft/libft.h"
# include "../builtin/builtin.h"
# include "../redirections/redirections.h"
# include "../common_utils/common_utils.h"
# include "../system_commands/system_commands.h"

/**
 * @brief Creates a single pipe node from a command.
 *
 * @param cmd Command to execute
 * @param head_tokens Tokens associated with the command
 * @return Created pipe node
 */
t_pipe	*create_pipe_node(t_cmd *cmd, t_token *head_tokens);

/**
 * @brief Creates a linked list of pipes from the command list.
 *
 * @param cmd List of commands
 * @param head_tokens List of tokens
 * @return Head of the pipe list
 */
t_pipe	*create_pipe_list(t_cmd *cmd, t_token *head_tokens);

/**
 * @brief Executes an external command using execve, redirecting stdout if necessary.
 *
 * @param fd File descriptor for output
 * @param dir Executable path
 * @param matrix_exe Argument matrix
 * @param env Shell environment
 */
void	ft_execution_execve(int *fd, char *dir, char **matrix_exe, t_env *env);

/**
 * @brief Executes a specific command in a pipeline.
 *
 * @param current Current pipe node
 * @param env Shell environment
 * @param token Command tokens
 * @param head Head of the pipe list
 */
void	ft_exe_pipe_cmd(t_pipe *current, t_env *env,
			t_token *token, t_pipe *head);

/**
 * @brief Executes a system command inside a pipeline.
 *
 * @param env Shell environment
 * @param cmd Command to execute
 * @return Exit status of the command
 */
int		ft_exe_syscmd_in_pipe(t_env *env, t_cmd *cmd);

/**
 * @brief Executes the entire pipeline of commands.
 *
 * @param head_pipe Head of the pipe list
 * @param env Shell environment
 */
void	execute_pipe(t_pipe *head_pipe, t_env *env);

/**
 * @brief Adds a node to the pipe list.
 *
 * @param head Head of the pipe list
 * @param new_token New node to add
 */
void	add_pipe(t_pipe **head, t_pipe *new_token);

#endif

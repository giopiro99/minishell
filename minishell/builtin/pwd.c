/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:32:58 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:50:05 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>

/**
 * @brief Closes the file descriptor if different from 1 and restores stdin if necessary.
 *
 * If the passed file descriptor is not standard output (1),
 * it closes it. If the command has redirected input,
 * it restores stdin to its saved value.
 *
 * @param fd Pointer to the file descriptor to close
 * @param cmd Pointer to the command structure
 * @param saved_stdin Pointer to the variable containing the original stdin value
 */
 static void	ft_too_close(int *fd, t_cmd *cmd, int *saved_stdin)
 {
	 if (*fd != 1)
		 close (*fd);
	 if (cmd->input)
		 restore_sdin(saved_stdin);
 }

 /**
  * @brief Implements the builtin pwd command.
  *
  * Obtains the current working directory and prints it on the file descriptor
  * specified by the command (redirected output if present).
  * Handles getcwd errors and restores input/output state.
  * Updates the environment's exit status.
  *
  * @param cmd Pointer to the current command structure
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the list of pipes, used to free memory in case of pipes
  */
 void	ft_pwd(t_cmd *cmd, t_env *env, t_pipe *head)
 {
	 char	*pwd;
	 int		fd;
	 int		saved_stdin;

	 saved_stdin = 0;
	 pwd = NULL;
	 fd = ft_for_fd(cmd, env, &saved_stdin);
	 if (fd == -1 || cmd->open_error)
		 return ;
	 pwd = getcwd(NULL, 0);
	 if (!pwd)
	 {
		 env->exit_status = 1;
		 perror("pwd");
		 return ;
	 }
	 ft_printf(fd, "%s\n", pwd);
	 free(pwd);
	 ft_too_close(&fd, cmd, &saved_stdin);
	 env->exit_status = 0;
	 if (cmd->pipe_flag)
		 free_all_pipe(head, env);
	 return ;
 }


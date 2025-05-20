/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:08:46 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:45:57 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Handles the case when exit is called without arguments or with "0".
 *
 * If there are no arguments, prints "exit", frees resources, and exits with the
 * current env->exit_status.
 * If the argument is exactly "0", prints "exit", frees resources, sets exit_status to 1,
 * and exits with code 0.
 *
 * @param env  Pointer to the shell environment
 * @param fd   File descriptor for output (stdout or file)
 * @param cmd  Pointer to the command structure with arguments
 */
 static	void	ft_exit_no_args_or_0(t_env *env, int fd, t_cmd *cmd)
 {
	 int	exit_status;

	 exit_status = 0;
	 if (!cmd->args[0])
	 {
		 exit_status = env->exit_status;
		 if (fd != 1)
			 close(fd);
		 return (free_all(cmd, env), ft_printf(1, "exit\n"), exit(exit_status));
	 }
	 else if (!ft_strcmp(cmd->args[0], "0"))
	 {
		 if (fd != 1)
			 close (fd);
		 env->exit_status = 1;
		 free_all(cmd, env);
		 ft_printf(1, "exit\n");
		 exit (0);
	 }
 }

 /**
  * @brief Checks if there are too many arguments for the exit command.
  *
  * If there is more than one argument, prints the error "too many arguments",
  * frees resources, and returns 1 to indicate error.
  * Otherwise returns 0.
  *
  * @param cmd   Pointer to the command structure
  * @param fd    File descriptor for output
  * @param head  Pointer to the pipe list for memory management
  * @param env   Pointer to the shell environment
  * @return int  1 if error, 0 otherwise
  */
 static int	ft_exit_too_args(t_cmd *cmd, int fd, t_pipe *head, t_env *env)
 {
	 if (cmd->args[1])
	 {
		 if (fd != 1)
			 close (fd);
		 if (cmd->pipe_flag)
			 free_all_pipe(head, env);
		 ft_printf(1, "minishell: exit: too many arguments\n");
		 return (1);
	 }
	 return (0);
 }

 /**
  * @brief Handles errors related to non-numeric or null exit arguments.
  *
  * If exit_n is zero (argument conversion failed), prints
  * "exit" and "exit: numeric argument required", frees resources, and exits.
  *
  * @param exit_n Converted exit argument value
  * @param cmd    Pointer to the command structure
  * @param env    Pointer to the shell environment
  * @param head   Pointer to the pipe list
  */
 static void	ft_error_exit(int exit_n, t_cmd *cmd, t_env *env, t_pipe *head)
 {
	 if (!exit_n)
	 {
		 if (!cmd->pipe_flag)
			 free_all(cmd, env);
		 else
			 free_all_pipe(head, env);
		 return (ft_printf(2, "exit\nexit: numeric argument required\n")
			 , exit(0));
	 }
 }

 /**
  * @brief Implementation of the builtin exit command.
  *
  * Handles redirections, argument checks, special cases like no arguments,
  * argument "0", too many arguments or non-numeric arguments.
  * Finally frees resources, prints "exit", and terminates the process with the correct code.
  *
  * @param cmd   Pointer to the command structure with arguments and flags
  * @param env   Pointer to the shell environment
  * @param head  Pointer to the pipe list for potential memory cleanup
  */
 void	ft_exit(t_cmd *cmd, t_env *env, t_pipe *head)
 {
	 long	exit_n;
	 int		fd;

	 exit_n = 0;
	 fd = check_value_fd(cmd->append, cmd->output, env);
	 if (ft_check_fd(fd, env) == -1 || cmd->open_error)
		 return ;
	 ft_exit_no_args_or_0(env, fd, cmd);
	 exit_n = ft_atol(cmd->args[0]);
	 ft_error_exit(exit_n, cmd, env, head);
	 if (ft_exit_too_args(cmd, fd, head, env))
		 return ;
	 if (exit_n > 255 || exit_n < 0)
		 exit_n = exit_n % 256;
	 env->exit_status = exit_n;
	 if (!cmd->pipe_flag)
		 free_all(cmd, env);
	 else
		 free_all_pipe(head, env);
	 ft_printf(1, "exit\n");
	 if (fd != 1)
		 close (fd);
	 exit(exit_n);
 }


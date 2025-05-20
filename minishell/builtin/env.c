/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:06:23 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 17:46:13 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Checks if an environment string contains a value after '='.
 *
 * Scans the string until the '=' character and checks if there is a non-empty value after it.
 *
 * @param str String to check (e.g. "KEY=VALUE")
 * @return int 1 if there is a value after '=', 0 otherwise
 */
 static int	check_value(char *str)
 {
	 int	i;

	 i = 0;
	 while (str && str[i] != '=')
		 i++;
	 if (str[i + 1] != '\0')
		 return (1);
	 return (0);
 }

 /**
  * @brief Prints valid environment variables to the specified file descriptor.
  *
  * Iterates over the env_copy array and prints only the variables that contain '=' and have a valid value.
  * Sets the exit status to 0.
  *
  * @param env Pointer to the shell environment structure
  * @param i   Pointer to the index from which to start iterating env_copy
  * @param fd  File descriptor to print to (stdout or file)
  */
 static void	print_args(t_env *env, int *i, int fd)
 {
	 while (env->env_copy[(*i)])
	 {
		 if (strchr(env->env_copy[(*i)], '=')
			 && check_value(env->env_copy[(*i)]))
			 ft_printf(fd, "%s\n", env->env_copy[(*i)]);
		 (*i)++;
	 }
	 env->exit_status = 0;
 }

 /**
  * @brief Handles errors for invalid arguments passed to the env command.
  *
  * If an argument is present, prints an error message and sets
  * exit_status to 1, marking the error through the passed flag by reference.
  *
  * @param env Pointer to the shell environment structure
  * @param cmd Pointer to the command structure
  * @param i   Pointer to the current argument index
  * @param error_flag Pointer to an error flag to set to 1 in case of error
  */
 static void	ft_env_error(t_env *env, t_cmd *cmd, int *i, int *error_flag)
 {
	 if (cmd->args[*i])
	 {
		 ft_printf(2, "env: %s not a valid identifier\n", cmd->args[*i]);
		 env->exit_status = 1;
		 *error_flag = 1;
	 }
 }

 /**
  * @brief Implementation of the builtin env command.
  *
  * Handles I/O redirection, validates arguments,
  * prints environment variables with valid values, and frees resources.
  *
  * @param cmd  Pointer to the command structure with arguments and flags
  * @param env  Pointer to the shell environment structure
  * @param head Pointer to the pipe list for memory management in case of pipes
  */
 void	ft_env(t_cmd *cmd, t_env *env, t_pipe *head)
 {
	 int	i;
	 int	fd;
	 int	saved_stdin;
	 int	error_flag;

	 i = 0;
	 error_flag = 0;
	 saved_stdin = 0;
	 fd = ft_for_fd(cmd, env, &saved_stdin);
	 if (fd == -1 || cmd->open_error)
		 return ;
	 ft_env_error(env, cmd, &i, &error_flag);
	 if (!error_flag)
		 print_args(env, &i, fd);
	 if (fd != 1)
		 close(fd);
	 if (cmd->input)
		 restore_sdin(&saved_stdin);
	 if (cmd->pipe_flag)
		 free_all_pipe(head, env);
	 return ;
 }


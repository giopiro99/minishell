/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:21:12 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:46:48 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdbool.h>

/**
 * @brief Executes export on a single argument.
 *
 * Checks if the identifier is valid. If not, prints an error and sets exit_status.
 * If the variable already exists in the environment, updates it only if it contains '=' (new value).
 * Otherwise adds a new variable to the environment.
 *
 * @param args  Array of export argument strings
 * @param i     Current index in the args array, passed by reference
 * @param env   Pointer to the shell environment structure
 */
 void	ft_exe_export(char **args, int *i, t_env *env)
 {
	 int		index;

	 index = -1;
	 if (ft_isspace(args[(*i)][0]) && args[(*i)][1] == '\0')
		 (*i)++;
	 if (!is_valid_identifier(args[(*i)]))
	 {
		 ft_printf(2, "export: %s not a valid identifier\n", args[(*i)]);
		 env->exit_status = 1;
	 }
	 else
	 {
		 index = ft_find_export_var(env, args[(*i)]);
		 if (index >= 0)
		 {
			 if (strchr(args[(*i)], '='))
			 {
				 free(env->env_copy[index]);
				 env->env_copy[index] = ft_strdup(args[(*i)]);
			 }
		 }
		 else
			 ft_add_var(env, args, i);
	 }
 }

 /**
  * @brief Helper function that prints a single exported variable
  *        formatted as "declare -x VAR="value"" if the variable has a value.
  *
  * @param env_sorted  Array of strings containing sorted environment variables
  * @param fd          File descriptor to write to (stdout or file)
  * @param i           Current index in the env_sorted array, passed by reference
  */
 static void	ft_too_write(char **env_sorted, int fd, int *i)
 {
	 int		j;
	 int		val_flag;
	 int		equals_flag;

	 j = 0;
	 val_flag = 0;
	 equals_flag = 0;
	 write(fd, "declare -x ", 11);
	 while (env_sorted[*i][j])
	 {
		 write(fd, &env_sorted[*i][j], 1);
		 if (env_sorted[*i][j] == '=' && !equals_flag)
		 {
			 val_flag = 1;
			 equals_flag = 1;
			 write(fd, "\"", 1);
		 }
		 j++;
	 }
	 if (val_flag == 1)
		 write(fd, "\"", 1);
	 write(fd, "\n", 1);
 }

 /**
  * @brief Handles the export command without arguments.
  *
  * Prints all environment variables sorted, using the export format.
  *
  * @param env  Pointer to the shell environment structure
  * @param fd   File descriptor to write to (stdout or file)
  */
 static void	ft_export_no_args(t_env *env, int fd)
 {
	 char	**env_sorted;
	 int		i;

	 i = 0;
	 env_sorted = ft_env_sorted(env->env_copy);
	 while (env_sorted[i])
	 {
		 ft_too_write(env_sorted, fd, &i);
		 i++;
	 }
 }

 /**
  * @brief Handles the export command with arguments.
  *
  * For each valid argument calls ft_exe_export to add/update
  * environment variables.
  *
  * @param cmd  Pointer to the command structure containing arguments
  * @param env  Pointer to the shell environment structure
  */
 static void	ft_export_with_args(t_cmd *cmd, t_env *env)
 {
	 int	i;

	 i = 0;
	 while (cmd->args[i])
	 {
		 if (cmd->args[i] && *cmd->args[i] != '\0')
			 ft_exe_export(cmd->args, &i, env);
		 i++;
	 }
	 env->exit_status = 0;
 }

 /**
  * @brief Main function handling the builtin export.
  *
  * Handles input/output redirections, prints or modifies environment variables,
  * closes file descriptors, restores stdin, and manages pipe memory cleanup.
  *
  * @param cmd   Pointer to the command structure with arguments and flags
  * @param env   Pointer to the shell environment structure
  * @param head  Pointer to the pipe list for potential memory cleanup
  */
 void	ft_export(t_cmd *cmd, t_env *env, t_pipe *head)
 {
	 int		fd;
	 int		saved_stdin;

	 saved_stdin = 0;
	 fd = ft_for_fd(cmd, env, &saved_stdin);
	 if (fd == -1 || cmd->open_error)
		 return ;
	 if (!cmd->args[0])
		 ft_export_no_args(env, fd);
	 else
		 ft_export_with_args(cmd, env);
	 if (fd != 1)
		 close(fd);
	 if (cmd->input)
		 restore_sdin(&saved_stdin);
	 env->exit_status = 0;
	 if (cmd->pipe_flag)
		 free_all_pipe(head, env);
 }


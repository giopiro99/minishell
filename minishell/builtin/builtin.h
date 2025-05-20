/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:22:18 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 17:45:29 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../libft/libft.h"
# include "../common_utils/common_utils.h"
# include "../redirections/redirections.h"

/**
 * @brief Builtin function for the cd (change directory) command.
 *
 * @param cmd  Pointer to the current command structure
 * @param env  Pointer to the shell environment structure
 * @param head Pointer to the pipe list (for memory management)
 * @return int 0 on success, -1 on error
 */
int		ft_cd(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Searches for a variable to export in the environment.
  *
  * @param env  Pointer to the shell environment structure
  * @param args Environment variable string to search for
  * @return int  index of the found variable or -1 if not found
  */
int		ft_find_export_var(t_env *env, char *args);

 /**
  * @brief Builtin function for the echo command.
  *
  * Prints the passed arguments to stdout or specified file descriptor.
  *
  * @param cmd Pointer to the command structure
  * @param token Pointer to the command line tokens
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the pipe list
  */
void	ft_echo(t_cmd *cmd, t_token *token, t_env *env, t_pipe *head);

 /**
  * @brief Builtin function for the pwd (print working directory) command.
  *
  * Prints the current working directory.
  *
  * @param cmd Pointer to the command structure
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the pipe list
  */
void	ft_pwd(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Builtin function for the unset command.
  *
  * Removes variables from the environment.
  *
  * @param cmd Pointer to the command structure
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the pipe list
  */
void	ft_unset(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Builtin function for the exit command.
  *
  * Terminates the shell with the specified exit code.
  *
  * @param cmd Pointer to the command structure
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the pipe list
  */
void	ft_exit(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Builtin function for the env command.
  *
  * Prints or modifies environment variables.
  *
  * @param cmd Pointer to the command structure
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the pipe list
  */
void	ft_env(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Builtin function for the export command.
  *
  * Adds or modifies variables in the export environment.
  *
  * @param cmd Pointer to the command structure
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the pipe list
  */
void	ft_export(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Frees all allocated resources associated with the command list and environment.
  *
  * @param cmd_list Pointer to the command list
  * @param env Pointer to the shell environment structure
  */
void	free_all(t_cmd *cmd_list, t_env *env);

 /**
  * @brief Sets or updates an environment variable.
  *
  * @param key Variable name
  * @param var Value to assign
  * @param env Pointer to the shell environment structure
  */
void	ft_setenv(const char *key, const char *var, t_env *env);

 /**
  * @brief Checks if a string is a valid identifier for environment variables.
  *
  * @param str String to check
  * @return true if valid, false otherwise
  */
bool	is_valid_identifier(char *str);

#endif


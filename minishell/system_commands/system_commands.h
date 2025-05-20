/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_commands.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:06:04 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:11:27 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEM_COMMANDS_H
# define SYSTEM_COMMANDS_H

# include "../libft/libft.h"
# include "../common_utils/common_utils.h"
# include "../redirections/redirections.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

/**
 * @brief Searches for the executable path of a command.
 *
 * @param env Shell environment containing environment variables
 * @param cmd Command structure with name and arguments
 * @return char* Full path to the command or NULL if not found
 */
 char	*ft_path_finder(t_env *env, t_cmd *cmd);

 /**
  * @brief Executes a system command by creating a child process.
  *
  * @param env Shell environment containing environment variables
  * @param cmd Command structure to execute
  * @return int 0 on success, error code otherwise
  */
 int		ft_exe_syscmd(t_env *env, t_cmd *cmd);

 /**
  * @brief Prepares the argument matrix for execve.
  *
  * @param cmd Command structure with arguments
  * @return char** Null-terminated argument matrix or NULL on error
  */
 char	**ft_mtx_cmd(t_cmd *cmd);


 #endif

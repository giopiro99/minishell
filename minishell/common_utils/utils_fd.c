/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:15:12 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:00:15 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Handles opening and validating file descriptors for command input/output.
 *
 * If the command has redirected input (cmd->input), it calls handle_input_redirection
 * to manage the input redirection and save the original stdin.
 * Then it determines the output file descriptor using check_value_fd based on append_flag and output.
 * Finally, it checks the validity of the file descriptor using ft_check_fd.
 *
 * @param cmd Pointer to the current command
 * @param env Pointer to the environment, to update exit_status on errors
 * @param saved_stdin Pointer to save the duplicated stdin if necessary
 * @return int valid file descriptor or -1 on error
 */
 int	ft_for_fd(t_cmd *cmd, t_env *env, int *saved_stdin)
 {
	 int	fd;

	 fd = 1;
	 if (cmd->input)
	 {
		 if (handle_input_redirection(cmd->input, saved_stdin) == -1)
			 return (-1);
	 }
	 fd = check_value_fd(cmd->append, cmd->output, env);
	 if (ft_check_fd(fd, env) == -1)
		 return (-1);
	 return (fd);
 }

 /**
  * @brief Checks if a file descriptor is valid.
  *
  * If fd == -1, indicates a file opening error, prints perror message,
  * sets exit_status, and returns -1.
  * Otherwise, returns 0.
  *
  * @param fd file descriptor to check
  * @param env environment to update exit_status
  * @return int 0 if valid, -1 if error
  */
 int	ft_check_fd(int fd, t_env *env)
 {
	 if (fd == -1)
	 {
		 perror("open");
		 env->exit_status = 1;
		 return (-1);
	 }
	 return (0);
 }

 /**
  * @brief Returns the correct file descriptor based on append flag and file.
  *
  * - If append_flag == 1: opens the file in append mode (creates if missing)
  * - If append_flag == 0: opens the file in truncation mode (creates if missing)
  * - If append_flag == -1: returns 1 (stdout)
  *
  * Always checks fd validity via ft_check_fd.
  *
  * @param append_flag flag indicating the open mode (append/trunc/-1)
  * @param file filename to operate on
  * @param env environment to update exit_status on error
  * @return int opened file descriptor or -1 on error
  */
 int	check_value_fd(int append_flag, char *file, t_env *env)
 {
	 int	fd;

	 fd = 0;
	 if (append_flag == 1)
	 {
		 fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0777);
		 if (ft_check_fd(fd, env) == -1)
			 return (-1);
	 }
	 else if (append_flag == 0)
	 {
		 fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		 if (ft_check_fd(fd, env) == -1)
			 return (-1);
	 }
	 else if (append_flag == -1)
		 fd = 1;
	 return (fd);
 }

 /**
  * @brief Checks if there is at least one HEREDOC token in the token list.
  *
  * Iterates through the token list and returns true if it finds a HEREDOC token.
  *
  * @param tokens pointer to the token list
  * @return true if a HEREDOC is found, false otherwise
  */
 bool	ft_found_heredoc(t_token *tokens)
 {
	 while (tokens)
	 {
		 if (tokens->type == HEREDOC)
			 return (true);
		 tokens = tokens->next;
	 }
	 return (false);
 }

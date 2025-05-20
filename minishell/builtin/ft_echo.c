/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:32:07 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:49:40 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>

/**
 * @brief Advances through the token list until reaching the requested argument index.
 *
 * Iterates through tokens until it finds the token corresponding to the argument with index arg_index,
 * considering token types ARGUMENT, ENV_VAR, QUOTE, EXIT_STATUS.
 *
 * @param token Pointer to the starting token
 * @param arg_index Index of the argument to reach
 * @return Pointer to the found token or NULL if not found
 */
 static t_token	*advance_token_to_arg(t_token *token, int arg_index)
 {
	 int	count;

	 count = -1;
	 while (token)
	 {
		 if (token->type == ARGUMENT || token->type == ENV_VAR
			 || token->type == QUOTE || token->type == EXIT_STATUS)
			 count++;
		 if (count == arg_index)
			 break ;
		 token = token->next;
	 }
	 return (token);
 }

 /**
  * @brief Checks for the presence of the -n option in the echo command.
  *
  * Analyzes the command arguments starting from index *i,
  * verifying if valid -n options are present (including multiple and concatenated ones),
  * and updates the index i beyond the valid options.
  *
  * @param cmd Pointer to the command structure
  * @param i Pointer to the argument index, updated upon exit
  * @return 1 if at least one valid -n option was found, 0 otherwise
  */
 static int	ft_check_newline_flag(t_cmd *cmd, int *i)
 {
	 int	j;
	 int	newline;

	 newline = 0;
	 while (cmd->args[*i])
	 {
		 if (cmd->args[*i][0] != '-' || cmd->args[*i][1] != 'n')
			 break ;
		 j = 2;
		 while (cmd->args[*i][j] == 'n')
			 j++;
		 if (cmd->args[*i][j] != '\0')
			 break ;
		 newline = 1;
		 (*i)++;
	 }
	 return (newline);
 }

 /**
  * @brief Writes the echo command arguments to the specified file descriptor.
  *
  * Properly handles spacing based on corresponding tokens,
  * ignoring GARBAGE and SPACES token types where appropriate.
  *
  * @param args Array of arguments to write
  * @param fd File descriptor to write the output to
  * @param token Pointer to the current token used to determine spacing
  */
 static void	ft_write_echo(char **args, int fd, t_token *token)
 {
	 int	i;

	 i = 0;
	 while (args[i])
	 {
		 write(fd, args[i], ft_strlen(args[i]));
		 i++;
		 token = token->next;
		 while (token && token->type == GARBAGE)
			 token = token->next;
		 if (token && token->type == SPACES && args[i])
			 write(fd, " ", 1);
		 while (token && token->type == SPACES)
			 token = token->next;
		 while (token && token->type != ARGUMENT && token->type != QUOTE
			 && token->type != ENV_VAR && token->type != EXIT_STATUS)
			 token = token->next;
	 }
 }

 /**
  * @brief Implements the builtin echo command.
  *
  * Handles multiple -n options, prints arguments separated by spaces,
  * and ends with a newline only if the -n option is not present.
  * Manages input/output redirections and updates the exit status.
  *
  * @param cmd Pointer to the current command structure
  * @param token Pointer to the list of tokens associated with the command
  * @param env Pointer to the shell environment structure
  * @param head Pointer to the list of pipes, used to free memory in case of pipes
  */
 void	ft_echo(t_cmd *cmd, t_token *token, t_env *env, t_pipe *head)
 {
	 int	fd;
	 int	saved_stdin;
	 int	i;
	 int	newline;

	 i = 0;
	 saved_stdin = 0;
	 newline = 0;
	 fd = ft_for_fd(cmd, env, &saved_stdin);
	 if (fd == -1 || cmd->open_error)
		 return ;
	 newline = ft_check_newline_flag(cmd, &i);
	 token = advance_token_to_arg(token, i);
	 ft_write_echo(&cmd->args[i], fd, token);
	 if (!newline)
		 write(fd, "\n", 1);
	 if (cmd->input)
		 restore_sdin(&saved_stdin);
	 if (fd != 1)
		 close(fd);
	 env->exit_status = 0;
	 if (cmd->pipe_flag)
		 free_all_pipe(head, env);
 }

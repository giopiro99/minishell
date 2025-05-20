/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:46:05 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:09:05 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"
#include <stdbool.h>
#include <stdio.h>
/**
 * @brief Handles input redirection from file or heredoc.
 *
 * If the type is REDIR_IN, opens the file for reading and saves the original stdin.
 * If the type is HEREDOC, creates a temporary file with heredoc content and redirects stdin to it.
 * Returns -1 on failure.
 *
 * @param tokens Current token containing the file name or heredoc
 * @param type Redirection type (REDIR_IN or HEREDOC)
 * @param env Shell environment, needed for heredoc
 * @param saved_stdin Pointer to save the original stdin file descriptor
 * @return int 0 on success, -1 on error
 */
 int	ft_handle_infile_redir(t_token *tokens, t_type type,
	t_env *env, int *saved_stdin)
{
	if (type == REDIR_IN)
	{
		if (handle_input_redirection(tokens->value, saved_stdin) == -1)
			return (-1);
	}
	else if (type == HEREDOC)
	{
		handle_heredoc(tokens, env);
		if (handle_input_redirection("temp_file.txt", saved_stdin) == -1)
			return (-1);
		if (tokens->next)
		{
			if (ft_found_heredoc(tokens->next))
				ft_end_heredoc(tokens);
		}
	}
	return (0);
}


/**
 * @brief Handles redirections without an associated command.
 *
 * Iterates tokens until it finds FILE_ or DELIMITER and opens the corresponding file
 * based on the redirection type (OUT, APPEND, IN, HEREDOC).
 * For redirected input, uses ft_handle_infile_redir.
 * Closes unnecessary opened file descriptors.
 * Prints an error and returns -1 if file opening fails.
 *
 * @param type Redirection type (REDIR_OUT, REDIR_APPEND, REDIR_IN, HEREDOC)
 * @param tokens List of tokens to get the file or delimiter from
 * @param env Shell environment
 * @return int Redirection type on success, -1 on error
 */
int	handle_redirection_without_command(t_type type,
	t_token *tokens, t_env *env)
{
	int	fd;
	int	saved_stdin;

	saved_stdin = 0;
	fd = -1; // initialize to -1 to indicate no file opened yet
	while (tokens && tokens->type != FILE_ && tokens->type != DELIMITER)
		tokens = tokens->next;
	if (!tokens || !tokens->value) // safeguard in case tokens is null or no value
		return (-1);
	if (type == REDIR_OUT)
		fd = open(tokens->value, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	else if (type == REDIR_IN || type == HEREDOC)
	{
		if (ft_handle_infile_redir(tokens, type, env, &saved_stdin) == -1)
			return (-1);
	}
	else if (type == REDIR_APPEND)
		fd = open(tokens->value, O_CREAT | O_APPEND | O_WRONLY, 0777);
	if (fd == -1 && (type == REDIR_OUT || type == REDIR_APPEND))
		return (perror("open"), -1);
	if (fd != 0 && fd != 1 && fd != -1)
		close(fd);
	if (type == REDIR_IN || type == HEREDOC)
		restore_sdin(&saved_stdin);
	return (type);
}

/**
 * @brief Handles redirection associated with a command.
 *
 * Calls handle_redirection_without_command to perform low-level redirection handling.
 * If the command exists, updates the input or output field of the command based on the type.
 * Also manages the assignment of the filename or heredoc in the command structure.
 * Returns the redirection type or -1 on error.
 *
 * @param cmd Pointer to the command structure (can be NULL)
 * @param type Redirection type to handle
 * @param tokens List of tokens with redirection information
 * @param env Shell environment
 * @return t_type Handled redirection type or -1 on error
 */
t_type	handle_redirection(t_cmd *cmd, t_type type, t_token *tokens, t_env *env)
{
	t_type	redir_type;

	redir_type = handle_redirection_without_command(type, tokens, env);
	if (redir_type == (t_type)-1)
		return (-1);
	if (!cmd)
		return (redir_type);
	while (tokens && (tokens->type != FILE_ && tokens->type != DELIMITER))
		tokens = tokens->next;
	if (ft_get_outfile_name(cmd, tokens, type))
		return (0);
	else if (type == REDIR_IN)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup(tokens->value);
	}
	else if (type == HEREDOC)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup("temp_file.txt");
	}
	return (redir_type);
}


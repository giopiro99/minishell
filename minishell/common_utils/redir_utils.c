/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:10:58 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:57:09 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Expands environment variables in a line and writes the result to a file descriptor.
 *
 * @param line input string from which to expand variables (e.g. "echo $HOME")
 * @param env environment variables structure (t_env)
 * @param fd file descriptor to write the expanded string to
 */
 void	ft_for_env_var(char *line, t_env *env, int fd)
 {
	 char	*expanded;

	 expanded = ft_expand_envvar(line, env);
	 write(fd, expanded, ft_strlen(expanded));
	 free(expanded);
 }

 /**
  * @brief Checks for following heredocs in tokens and removes the temporary file.
  *
  * If the next token is of type HEREDOC, deletes the temporary file "temp_file.txt".
  * Used for heredoc handling in the shell.
  *
  * @param tokens linked list of tokens
  */
 void	ft_end_heredoc(t_token *tokens)
 {
	 if (tokens->next && ft_found_heredoc(tokens->next))
		 unlink("temp_file.txt");
 }

 /**
  * @brief Assigns the output file name and append flag based on redirection type.
  *
  * If type is REDIR_OUT, sets cmd->output to the filename and append = 0.
  * If type is REDIR_APPEND, sets cmd->output and append = 1.
  *
  * @param cmd command structure to modify
  * @param tokens token containing the filename (tokens->value)
  * @param type redirection type (REDIR_OUT or REDIR_APPEND)
  * @return true if the filename was assigned, false otherwise
  */
 bool	ft_get_outfile_name(t_cmd *cmd, t_token *tokens, t_type type)
 {
	 if (type == REDIR_OUT)
	 {
		 if (cmd->output)
			 free(cmd->output);
		 cmd->output = ft_strdup(tokens->value);
		 cmd->append = 0;
		 return (true);
	 }
	 else if (type == REDIR_APPEND)
	 {
		 if (cmd->output)
			 free(cmd->output);
		 cmd->output = ft_strdup(tokens->value);
		 cmd->append = 1;
		 return (true);
	 }
	 return (false);
 }

 /**
  * @brief Gets the filename from a token based on redirection type and assigns it to the command.
  *
  * Finds the token representing a FILE_ or DELIMITER (for heredoc) and assigns the filename to the command:
  * - REDIR_OUT or REDIR_APPEND uses ft_get_outfile_name()
  * - REDIR_IN assigns cmd->input to the filename
  * - HEREDOC assigns cmd->input to "temp_file.txt" (temporary file used for heredoc)
  *
  * @param head head of the token list
  * @param tokens pointer to the current token (not used as input but reset to head)
  * @param cmd command structure to assign the filename to
  * @param type redirection type (REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC)
  * @return always returns 0
  */
 int	ft_get_file_name(t_token *head, t_token *tokens, t_cmd *cmd, t_type type)
 {
	 tokens = head;
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
	 return (0);
 }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:47:39 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:09:31 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "../common_utils/common_utils.h"

/**
 * @brief Handles a redirection inside a command.
 *
 * Depending on the redirection type (>, >>, <, <<),
 * performs the appropriate operation possibly modifying
 * the command's file descriptors.
 *
 * @param cmd Current command structure
 * @param type Redirection type to handle
 * @param tokens List of tokens associated with the redirection
 * @param env Shell environment structure
 * @return The handled redirection type or ERROR on failure
 */
 t_type	handle_redirection(t_cmd *cmd, t_type type,
	t_token *tokens, t_env *env);

/**
* @brief Handles a redirection even without an associated command.
*
* Useful for cases like a standalone here-document, e.g. `<< DELIM`.
*
* @param type Redirection type
* @param tokens List of tokens associated with the redirection
* @param env Shell environment
* @return 0 on success, -1 on error
*/
int		handle_redirection_without_command(t_type type,
	t_token *tokens, t_env *env);

/**
* @brief Handles the full input process for a here-document.
*
* Opens a temporary file and fills it by reading from stdin until
* the delimiter is reached. Supports interruption via signals like SIGINT.
*
* @param tokens List of tokens of the current command
* @param env Pointer to the shell environment structure
* @return 0 on success, -1 on error
*/
int		handle_heredoc(t_token *tokens, t_env *env);

/**
* @brief Returns the value of the token used as here-document delimiter.
*
* Searches the token list for the first token of type DELIMITER and returns its value.
*
* @param tokens List of tokens to analyze
* @return The delimiter value or NULL if not found
*/
char	*ft_found_delimiter(t_token *tokens);

/**
* @brief Executes a single read-write cycle of the here-document.
*
* Reads a line from stdin, checks if it is the delimiter, otherwise
* writes it to the file. Also handles environment variables and interruption
* via signals.
*
* @param tokens List of tokens containing the delimiter and other info
* @param env Shell environment
* @param fd File descriptor of the temporary file
* @return true if the heredoc should be interrupted, false otherwise
*/
bool	ft_exe_heredoc(t_token *tokens, t_env *env, int *fd);

#endif


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:33:43 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:37:04 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "../common_utils/common_utils.h"

/*
 * Tokenizes the input string into a linked list of tokens.
 * Returns a pointer to the head of the token list.
 */
 t_token	*tokenize(char *input);

 /*
  * Determines the token type based on its value, presence of quotes,
  * and whether the token is part of a command.
  */
 t_type	get_token_type(char *value, int q_flag, int is_command);

 /*
  * Adds a new token to the linked list of tokens.
  */
 void	add_token(t_token **head, t_token *new_token);

 /*
  * Assigns appropriate types to tokens (e.g., COMMAND, ARGUMENT, FILE_, etc.)
  * based on the context and token sequence.
  */
 void	assign_token_types(t_token *tokens);

 /*
  * Correctly marks SPACES tokens relative to adjacent tokens.
  */
 void	mark_sign_spaces(t_token *tokens);

 /*
  * Merges consecutive ARGUMENT tokens in the token list for the 'export' command.
  */
 void	ft_merge_tokens_export(t_token **tokens);

 /*
  * Merges multiple '-n' option tokens in ARGUMENT tokens for the 'echo' command.
  */
 void	ft_merge_tokens_echo(t_token **tokens);

 /*
  * Checks the syntax of the token list and reports errors.
  * Returns true if a syntax error is present, false otherwise.
  */
 bool	check_sintax(t_token *tokens);

 /*
  * Checks if the token list consists only of spaces or useless tokens (GARBAGE).
  * Returns true if so, false otherwise.
  */
 bool	check_only_spaces(t_token *tokens);

 /*
  * Extracts strings enclosed in quotes, copying them into a buffer.
  * Updates input and buffer indices, returning a type flag.
  */
 int		ft_for_quotes(char *input, int *i, int *j, char *buffer);

 /* Specific token handling functions used during parsing */

 /*
  * Parses strings enclosed in quotes.
  * Returns true if parsing is successful, false on error.
  */
 bool	handle_quotes(char *input, int *i, char *buffer, t_token **tokens);

 /*
  * Handles special characters like pipes and redirections during tokenization.
  * Returns true if parsing is successful, false on error.
  */
 bool	handle_special_chars(char *input, int *i, char *buffer, t_token **tokens);

 /*
  * Handles tokenization of normal words (non-special, non-quoted).
  * Returns true if parsing is successful, false on error.
  */
 bool	handle_word(char *input, int *i, char *buffer, t_token **tokens);

 /* Syntax checking functions specific to token types */

 /*
  * Checks the syntax correctness of redirection tokens.
  */
 bool	check_redir(t_token *tmp);

 /*
  * Checks the syntax correctness of heredoc tokens.
  */
 bool	check_heredoc(t_token *tmp);

 /*
  * Checks the syntax correctness of pipe tokens.
  */
 bool	check_pipe(t_token *tmp);

 /*
  * Checks the syntax correctness of command tokens.
  */
 bool	check_command(t_token *tmp);

 /*
  * Checks if tokens are only spaces or useless tokens.
  */
 bool	check_only_spaces(t_token *tokens);


#endif


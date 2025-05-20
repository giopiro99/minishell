/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:29:27 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:11:58 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Marks and manages SPACES tokens in relation to ARGUMENT, COMMAND, QUOTE, ENV_VAR, and PIPE tokens.
 *
 * Iterates through the token list and updates the state between arguments,
 * calling ft_assign_spaces to correctly assign spaces.
 *
 * @param tokens List of tokens to process.
 */
 void	mark_sign_spaces(t_token *tokens)
 {
	 t_token	*prev;
	 int		between_arg;

	 prev = NULL;
	 between_arg = 0;
	 while (tokens)
	 {
		 if (tokens->type == ARGUMENT || tokens->type == COMMAND
			 || tokens->type == QUOTE || tokens->type == ENV_VAR
			 || tokens->type == PIPE)
		 {
			 if (between_arg)
				 between_arg = 0;
			 prev = tokens;
		 }
		 else if (tokens->type == SPACES)
			 ft_assign_spaces(prev, tokens, &between_arg);
		 tokens = tokens->next;
	 }
 }

 /**
  * @brief Correctly assigns types to tokens related to redirections and here-documents.
  *
  * If the redirection flag is active, searches for the next ARGUMENT token and marks it as FILE_.
  * If the token is HEREDOC, searches for the next ARGUMENT token and marks it as DELIMITER.
  * Also manages activation/deactivation of redir_flag and is_first flags.
  *
  * @param redir_flag Pointer to the redirection flag.
  * @param tokens Current token to process.
  * @param is_first Pointer to the flag indicating if it is the first command.
  * @return 0 if assignment succeeded, -1 otherwise.
  */
 static int	assign_redir(int *redir_flag, t_token *tokens, int *is_first)
 {
	 if (*redir_flag)
	 {
		 while (tokens->next && tokens->type != ARGUMENT)
			 tokens = tokens->next;
		 tokens->type = FILE_;
		 *redir_flag = 0;
		 *is_first = 1;
		 return (0);
	 }
	 else if (tokens->type == HEREDOC)
	 {
		 while (tokens->next && tokens->type != ARGUMENT)
			 tokens = tokens->next;
		 if (tokens->type != HEREDOC && !is_space(tokens))
			 tokens->type = DELIMITER;
		 *is_first = 1;
		 *redir_flag = 0;
		 return (0);
	 }
	 else if (is_redir(tokens))
		 return (*redir_flag = 1, *is_first = 0, 0);
	 return (-1);
 }

 /**
  * @brief Assigns COMMAND or ARGUMENT type to a token.
  *
  * If it is the first valid token, assigns COMMAND; otherwise, ARGUMENT.
  * Updates the is_first flag.
  *
  * @param tokens Token to assign.
  * @param is_first Pointer to the flag indicating if it is the first command.
  */
 static void	assign_cmd(t_token *tokens, int *is_first)
 {
	 if (tokens->type == COMMAND || tokens->type == ARGUMENT)
	 {
		 if (*is_first)
			 tokens->type = COMMAND;
		 else
			 tokens->type = ARGUMENT;
		 *is_first = 0;
	 }
 }

 /**
  * @brief Handles the type assignment for PIPE tokens.
  *
  * If the token is PIPE, checks the next token:
  * - if it is not PIPE, recursively calls assign_token_types to assign types.
  *
  * @param tokens Token to check.
  * @return 0 if it was a PIPE token and processed, -1 otherwise.
  */
 static int	assign_pipe(t_token *tokens)
 {
	 if (tokens->type == PIPE)
	 {
		 if (tokens->next)
			 tokens = tokens->next;
		 if (tokens->type != PIPE)
			 assign_token_types(tokens);
		 return (0);
	 }
	 return (-1);
 }

 /**
  * @brief Assigns correct types to the tokens in the list (COMMAND, ARGUMENT, FILE_, DELIMITER, PIPE).
  *
  * Iterates through the entire token list and uses helper functions
  * to assign correct types depending on the syntactic context.
  *
  * @param tokens List of tokens to process.
  */
 void	assign_token_types(t_token *tokens)
 {
	 int	is_first;
	 int	redir_flag;
	 int	first_space;

	 is_first = 1;
	 redir_flag = 0;
	 first_space = 1;
	 while (tokens)
	 {
		 if (tokens->type == SPACES || tokens->type == QUOTE)
		 {
			 if (tokens->type == QUOTE)
				 is_first = 0;
			 if (tokens->next)
				 tokens = tokens->next;
			 else
				 break ;
			 continue ;
		 }
		 assign_redir(&redir_flag, tokens, &is_first);
		 assign_cmd(tokens, &is_first);
		 if (assign_pipe(tokens) == 0)
			 break ;
		 tokens = tokens->next;
	 }
 }

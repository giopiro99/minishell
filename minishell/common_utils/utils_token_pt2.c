/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_pt2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:04:10 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:01:17 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Checks if the token represents a redirection.
 *
 * Considers token types REDIR_IN, REDIR_OUT, and REDIR_APPEND.
 *
 * @param tokens Pointer to the token to check
 * @return true If the token is a redirection
 * @return false Otherwise
 */
 bool	is_redir(t_token *tokens)
 {
	 if (tokens->type == REDIR_IN
		 || tokens->type == REDIR_OUT || tokens->type == REDIR_APPEND)
		 return (true);
	 return (false);
 }

 /**
  * @brief Checks if the token is a valid argument.
  *
  * Considers ARGUMENT, ENV_VAR, QUOTE, and EXIT_STATUS as arguments.
  *
  * @param tokens Pointer to the token to check
  * @return true If the token is an argument
  * @return false Otherwise
  */
 bool	is_argument(t_token *tokens)
 {
	 if (tokens->type == ARGUMENT || tokens->type == ENV_VAR
		 || tokens->type == QUOTE || tokens->type == EXIT_STATUS)
		 return (true);
	 return (false);
 }

 /**
  * @brief Checks if the token represents whitespace or invalid characters (garbage).
  *
  * Checks token types SPACES and GARBAGE.
  *
  * @param tokens Pointer to the token to check
  * @return true If the token is whitespace or garbage
  * @return false Otherwise
  */
 bool	is_space(t_token *tokens)
 {
	 if (tokens->type == SPACES || tokens->type == GARBAGE)
		 return (true);
	 return (false);
 }

 /**
  * @brief Checks if a string is a valid sequence of '-n' for the echo command.
  *
  * Validates that the string starts with "-n" followed by zero or more 'n's.
  * Examples: "-n", "-nn", "-nnn" are valid; "-nm" is not.
  *
  * @param str String to check
  * @return true If it is a valid '-n' sequence
  * @return false Otherwise
  */
 bool	is_n_sequence(char *str)
 {
	 int	i;

	 i = 2;
	 if (!str || str[0] != '-' || str[1] != 'n')
		 return (false);
	 while (str[i])
	 {
		 if (str[i] != 'n')
			 return (false);
		 i++;
	 }
	 return (true);
 }

 /**
  * @brief Assigns the token type for space tokens based on surrounding context.
  *
  * - If the previous token is an ARGUMENT and is a '-n' sequence, sets the current token as GARBAGE.
  * - If the previous token is ARGUMENT, ENV_VAR, or QUOTE, and the next token is also one of those,
  *   and no space has been assigned between arguments yet, sets the current token as SPACES,
  *   otherwise sets it as GARBAGE.
  * - In all other cases, sets the token type as GARBAGE.
  *
  * @param prev Previous token (can be NULL)
  * @param tokens Current token to modify
  * @param between_arg Pointer to an integer flag indicating if a space between arguments has already been found (0 or 1)
  */
 void	ft_assign_spaces(t_token *prev, t_token *tokens, int *between_arg)
 {
	 if (prev && prev->type == ARGUMENT && is_n_sequence(prev->value))
		 tokens->type = GARBAGE;
	 else if (prev && ((prev->type == ARGUMENT || prev->type == ENV_VAR
				 || prev->type == QUOTE)
			 && tokens->next && (tokens->next->type == ARGUMENT
				 || tokens->next->type == ENV_VAR
				 || tokens->next->type == QUOTE)))
	 {
		 if (!*between_arg)
		 {
			 tokens->type = SPACES;
			 *between_arg = 1;
		 }
		 else
			 tokens->type = GARBAGE;
	 }
	 else
		 tokens->type = GARBAGE;
 }


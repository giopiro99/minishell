/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_merge_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:57:45 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:13:13 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Checks if a string consists only of 'n' characters.
 *
 * Useful to determine if an option like `-nnn` is valid for `echo -n`.
 *
 * @param str The string to check.
 * @return 1 if it contains only 'n', otherwise 0.
 */
 static int	ft_is_only_n(char *str)
 {
	 while (*str)
	 {
		 if (*str != 'n')
			 return (0);
		 str++;
	 }
	 return (1);
 }

 /**
  * @brief Merges two tokens by concatenating their values.
  *
  * Updates the value of the `curr` token and removes the `next` token from the list.
  *
  * @param tokens Pointer to the head of the token list.
  * @param curr The current token to update.
  * @param next The next token to merge and remove.
  */
 static void	ft_merge_n_tokens(t_token **tokens, t_token *curr, t_token *next)
 {
	 char	*merged;
	 t_token	*to_remove;

	 merged = ft_strjoin(curr->value, next->value);
	 free(curr->value);
	 curr->value = merged;
	 to_remove = next;
	 remove_token_node(tokens, to_remove);
 }

 /**
  * @brief Finds and merges all consecutive tokens like `-n`, `-nn`, etc.
  *        after the initial valid `-n` token.
  *
  * Iterates through the token list and while it finds tokens starting with `-n*`,
  * concatenates them into the first initial `-n` token.
  *
  * @param tokens Pointer to the head of the token list.
  * @param curr The initial `-n` token.
  */
 static void	ft_merge_subsequent_n(t_token **tokens, t_token *curr)
 {
	 t_token	*next;

	 next = curr->next;
	 while (next && next->type == ARGUMENT)
	 {
		 if (ft_is_only_n(next->value))
		 {
			 ft_merge_n_tokens(tokens, curr, next);
			 next = curr->next;
		 }
		 else
			 break ;
	 }
 }

 /**
  * @brief Checks if a token represents a valid `-n` option for the `echo` command.
  *
  * A valid option is in the form `-n`, `-nn`, etc. and has type ARGUMENT.
  *
  * @param curr The token to verify.
  * @return 1 if it is a valid `-n` option, otherwise 0.
  */
 static int	ft_check_n_option(t_token *curr)
 {
	 char	*pos;

	 if (!curr || curr->type != ARGUMENT)
		 return (0);
	 if (ft_strncmp(curr->value, "-", 1) != 0)
		 return (0);
	 pos = curr->value + 1;
	 return (ft_is_only_n(pos));
 }

 /**
  * @brief Handles merging multiple `-n`, `-nn`, etc. tokens for the `echo` command.
  *
  * If the first token is "echo" and the next one is a `-n` option,
  * merges all subsequent `-n*` options into a single token.
  * This allows correct handling of syntax like `echo -n -nn hello`.
  *
  * @param tokens Pointer to the head of the token list.
  */
 void	ft_merge_tokens_echo(t_token **tokens)
 {
	 t_token	*curr;

	 if (!tokens || !(*tokens))
		 return ;
	 curr = *tokens;
	 if (!curr || !curr->next || ft_strcmp(curr->value, "echo") != 0)
		 return ;
	 curr = curr->next;
	 while (curr && (curr->type == SPACES || curr->type == GARBAGE))
		 curr = curr->next;
	 if (!ft_check_n_option(curr))
		 return ;
	 ft_merge_subsequent_n(tokens, curr);
 }

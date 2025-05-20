/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_merge_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:46:19 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:13:40 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <string.h>
#include "tokens.h"

/**
 * @brief Merges the current token's value with the next token's value.
 *
 * Uses `ft_strjoin` to concatenate the values of the two tokens and updates
 * the pointer `to_delete` with the `next` token, which will be removed afterwards.
 *
 * @param to_delete Pointer to the token to delete (will be set to `next`)
 * @param next The next token to merge
 * @param merged Pointer to the string containing the merged result
 */
 static void	ft_merger(t_token **to_delete, t_token *next, char **merged)
 {
	 char	*tmp;

	 tmp = ft_strjoin(*merged, next->value);
	 free(*merged);
	 *merged = tmp;
	 *to_delete = next;
 }

 /**
  * @brief Merges consecutive ARGUMENT tokens into a single token,
  *        used for the `export` command.
  *
  * Starting from an ARGUMENT token, concatenates subsequent tokens
  * as long as they are also ARGUMENTs, updating the current token's value
  * and removing the subsequent tokens from the list.
  *
  * @param curr Pointer to the current token (initially the first ARGUMENT after "export")
  * @param next Working pointer to the next token
  * @param tokens Pointer to the head of the token list
  */
 static void	ft_merge_word_export(t_token *curr, t_token *next, t_token **tokens)
 {
	 char	*merged;
	 char	*tmp;
	 t_token	*to_delete;

	 to_delete = NULL;
	 tmp = NULL;
	 while (curr)
	 {
		 if (curr->type != ARGUMENT)
		 {
			 curr = curr->next;
			 continue ;
		 }
		 merged = ft_strdup(curr->value);
		 next = curr->next;
		 while (next && (next->type == ARGUMENT))
		 {
			 ft_merger(&to_delete, next, &merged);
			 next = next->next;
			 remove_token_node(tokens, to_delete);
		 }
		 free(curr->value);
		 curr->value = merged;
		 curr = curr->next;
	 }
 }

 /**
  * @brief Checks if the current command is `export` and,
  *        if so, merges the following ARGUMENT tokens.
  *
  * This handles cases where `export` receives a value with spaces not enclosed in quotes.
  * Example: `export VAR=hello world` → `VAR=hello world` should be merged into a single token.
  *
  * @param tokens Pointer to the head of the token list
  */
 void	ft_merge_tokens_export(t_token **tokens)
 {
	 t_token	*curr;
	 t_token	*next;

	 curr = NULL;
	 next = NULL;
	 if (!(*tokens) || !tokens)
		 return ;
	 curr = *tokens;
	 if ((curr->next && (curr->next->type != ARGUMENT
				 && curr->next->type != SPACES && curr->next->type != GARBAGE))
		 || ft_strcmp(curr->value, "export") != 0)
		 return ;
	 curr = curr->next;
	 ft_merge_word_export(curr, next, tokens);
 }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:00:05 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:14:45 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <readline/chardefs.h>
#include <stdbool.h>

/**
 * @brief Assigns types to tokens and applies command-specific rules.
 *
 * This function processes the token list to:
 * - assign types (e.g., ARGUMENT, REDIRECTION, etc.),
 * - mark special spaces,
 * - correctly merge tokens in `export` and `echo` commands.
 *
 * @param tokens List of tokens to process
 */
 void	ft_define_tokens(t_token *tokens)
 {
	 assign_token_types(tokens);
	 mark_sign_spaces(tokens);
	 ft_merge_tokens_export(&tokens);
	 ft_merge_tokens_echo(&tokens);
 }

 /**
  * @brief Extracts and builds a single token from the user input.
  *
  * Depending on the current character, handles:
  * - spaces,
  * - quotes,
  * - special characters (| < >),
  * - plain words.
  *
  * @param input Input string to analyze
  * @param i Pointer to the current index in the input
  * @param tokens Pointer to the token list to fill
  * @param buffer Temporary buffer for building words
  * @return 0 on success, -1 on error
  */
 int	ft_get_tokens(char *input, int *i, t_token **tokens, char **buffer)
 {
	 while (ft_isspace(input[(*i)]))
	 {
		 add_token(tokens, new_token(" ", SPACES));
		 (*i)++;
	 }
	 if (input[(*i)] == '\0')
		 return (0);
	 if (input[(*i)] == '\'' || input[(*i)] == '"')
	 {
		 if (!handle_quotes(input, i, *buffer, tokens))
			 return (-1);
	 }
	 else if (input[(*i)] == '|' || input[(*i)] == '<' || input[(*i)] == '>')
	 {
		 if (!handle_special_chars(input, i, *buffer, tokens))
			 return (-1);
	 }
	 else
	 {
		 if (!handle_word(input, i, *buffer, tokens))
			 return (-1);
	 }
	 return (0);
 }

 /**
  * @brief Tokenizes the entire user input string.
  *
  * Allocates a temporary buffer, repeatedly calls `ft_get_tokens`
  * to build the token list, then calls `ft_define_tokens`
  * to finalize processing.
  *
  * @param input User input string to tokenize
  * @return Pointer to the generated token list, or NULL on error
  */
 t_token	*tokenize(char *input)
 {
	 int		i;
	 char	*buffer;
	 t_token	*tokens;

	 i = 0;
	 tokens = NULL;
	 buffer = malloc(sizeof(char) * (ft_strlen(input) + 1));
	 if (!buffer)
		 return (NULL);
	 while (ft_isspace(input[i]))
		 i++;
	 while (input[i])
	 {
		 if (ft_get_tokens(input, &i, &tokens, &buffer) == -1)
			 return (NULL);
		 if (input[i] == '\0')
			 break ;
	 }
	 free(buffer);
	 ft_define_tokens(tokens);
	 return (tokens);
 }

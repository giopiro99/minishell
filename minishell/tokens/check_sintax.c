/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sintax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:54:34 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:12:50 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Checks for syntax errors on a single token.
 *
 * Verifies the syntactic correctness of the given token based on its type.
 * If the token is a redirection, here-document, pipe, or command,
 * calls the corresponding specific check function.
 *
 * @param tmp The token to check.
 * @return true if a syntax error was detected, false otherwise.
 */
 bool	ft_sintax_errors(t_token *tmp)
 {
	 if (is_redir(tmp))
	 {
		 if (check_redir(tmp) == true)
			 return (true);
	 }
	 else if (tmp->type == HEREDOC)
	 {
		 if (check_heredoc(tmp) == true)
			 return (true);
	 }
	 else if (tmp->type == PIPE)
	 {
		 if (check_pipe(tmp) == true)
			 return (true);
	 }
	 else if (tmp->type == COMMAND)
	 {
		 if (check_command(tmp) == true)
			 return (true);
	 }
	 return (false);
 }


 /**
  * @brief Checks the syntax of the entire token list.
  *
  * Skips space tokens at the start of the list.
  * If the list starts with a pipe, prints a syntax error.
  * Then iterates over all tokens and checks syntax via ft_sintax_errors.
  *
  * @param tokens Pointer to the first token in the list.
  * @return true if a syntax error was detected, false if syntax is correct.
  */
 bool	check_sintax(t_token *tokens)
 {
	 t_token	*tmp;

	 tmp = tokens;
	 while (tmp->next && (is_space(tmp)))
		 tmp = tmp->next;
	 if (tmp->type == PIPE)
	 {
		 error_ex("minishell: syntax error near unexpected token `|'\n");
		 return (true);
	 }
	 while (tmp)
	 {
		 while (tmp->next && (is_space(tmp)))
			 tmp = tmp->next;
		 if (ft_sintax_errors(tmp) == true)
			 return (true);
		 tmp = tmp->next;
	 }
	 return (tokens = tmp, false);
 }

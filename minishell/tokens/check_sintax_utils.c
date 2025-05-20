/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sintax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:38:25 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:12:26 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <stdbool.h>

/**
 * @brief Checks the syntactic correctness of a redirection.
 *
 * Verifies that after the redirection token there is a valid FILE_ token.
 * Ignores tokens of type SPACES and GARBAGE.
 * If the next token is missing or invalid, prints an error.
 *
 * @param tmp The redirection token to check.
 * @return true if there is a syntax error, false otherwise.
 */
 bool	check_redir(t_token *tmp)
 {
	 if (tmp->next)
	 {
		 tmp = tmp->next;
		 while (tmp->next && (tmp->type == SPACES || tmp->type == GARBAGE))
			 tmp = tmp->next;
		 if (tmp->type != FILE_ || tmp->value[0] == '>' || tmp->value[0] == '<')
		 {
			 error_ex("minishell:syntax error near unexpected token newline\n");
			 return (true);
		 }
	 }
	 else if (!tmp->next)
	 {
		 error_ex("minishell:syntax error near unexpected token newline\n");
		 return (true);
	 }
	 return (false);
 }

 /**
  * @brief Checks the syntactic correctness of a here-document.
  *
  * Verifies that after the HEREDOC token there is a valid DELIMITER token.
  * Ignores tokens of type SPACES and GARBAGE.
  * If the next token is missing or invalid, prints an error.
  *
  * @param tmp The HEREDOC token to check.
  * @return true if there is a syntax error, false otherwise.
  */
 bool	check_heredoc(t_token *tmp)
 {
	 if (tmp->next)
	 {
		 tmp = tmp->next;
		 while (tmp->next && (tmp->type == SPACES || tmp->type == GARBAGE))
			 tmp = tmp->next;
		 if (tmp->type != DELIMITER
			 || (!ft_isalnum(tmp->value[0]) && ft_strncmp("_", tmp->value, 1)))
		 {
			 error_ex("minishell:syntax error near unexpected token newline\n");
			 return (true);
		 }
	 }
	 else if (!tmp->next)
	 {
		 error_ex("minishell:syntax error near unexpected token newline\n");
		 return (true);
	 }
	 return (false);
 }

 /**
  * @brief Checks the syntactic correctness of a pipe.
  *
  * Verifies that after the PIPE token there is a valid token: COMMAND, redirection, or HEREDOC.
  * Ignores tokens of type SPACES and GARBAGE.
  * If the next token is missing or invalid, prints an error.
  *
  * @param tmp The PIPE token to check.
  * @return true if there is a syntax error, false otherwise.
  */
 bool	check_pipe(t_token *tmp)
 {
	 if (tmp->next)
	 {
		 tmp = tmp->next;
		 while (tmp->next && (tmp->type == SPACES || tmp->type == GARBAGE))
			 tmp = tmp->next;
		 if (tmp->type != COMMAND && (!is_redir(tmp)) && tmp->type != HEREDOC)
		 {
			 error_ex("minishell: syntax error near unexpected token `|'\n");
			 return (true);
		 }
	 }
	 else if (!tmp->next)
	 {
		 error_ex("minishell: syntax error near unexpected token `|'\n");
		 return (true);
	 }
	 return (false);
 }

 /**
  * @brief Checks the syntax related to a command.
  *
  * If the next token exists and is not a space, prints a command not found error.
  *
  * @param tmp The COMMAND token to check.
  * @return true if there is an error, false otherwise.
  */
 bool	check_command(t_token *tmp)
 {
	 t_token	*prev;

	 prev = tmp;
	 if (tmp->next)
	 {
		 tmp = tmp->next;
		 if (!is_space(tmp))
		 {
			 ft_printf(2, "minishell: command not found %s%s\n",
				 prev->value, tmp->value);
			 return (true);
		 }
		 return (false);
	 }
	 return (false);
 }

 /**
  * @brief Checks if the token list contains only spaces or garbage tokens.
  *
  * @param tokens List of tokens to check.
  * @return true if all tokens are SPACES or GARBAGE, false otherwise.
  */
 bool	check_only_spaces(t_token *tokens)
 {
	 t_token	*tmp;

	 tmp = tokens;
	 while (tmp)
	 {
		 if (tmp->type != SPACES && tmp->type != GARBAGE)
			 return (false);
		 tmp = tmp->next;
	 }
	 return (true);
 }

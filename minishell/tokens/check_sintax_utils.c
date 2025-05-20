/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sintax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:38:25 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:46:30 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <stdbool.h>

/**
 * @brief Controlla la correttezza sintattica di una redirezione.
 *
 * Verifica che dopo il token di redirezione ci sia un token valido di tipo FILE_.
 * Ignora token di tipo SPACES e GARBAGE.
 * Se manca il token successivo o è errato, stampa un errore.
 *
 * @param tmp Il token di redirezione da controllare.
 * @return true se c'è un errore di sintassi, false altrimenti.
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
 * @brief Controlla la correttezza sintattica di un here-document.
 *
 * Verifica che dopo il token HEREDOC ci sia un token di tipo DELIMITER valido.
 * Ignora token di tipo SPACES e GARBAGE.
 * Se manca il token successivo o è errato, stampa un errore.
 *
 * @param tmp Il token HEREDOC da controllare.
 * @return true se c'è un errore di sintassi, false altrimenti.
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
 * @brief Controlla la correttezza sintattica di una pipe.
 *
 * Verifica che dopo il token PIPE ci sia un token valido: COMMAND, redirezione o HEREDOC.
 * Ignora token di tipo SPACES e GARBAGE.
 * Se manca il token successivo o è errato, stampa un errore.
 *
 * @param tmp Il token PIPE da controllare.
 * @return true se c'è un errore di sintassi, false altrimenti.
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
 * @brief Controlla la sintassi relativa a un comando.
 *
 * Se il token successivo esiste e non è uno spazio, stampa un errore di comando non trovato.
 *
 * @param tmp Il token COMMAND da controllare.
 * @return true se c'è un errore, false altrimenti.
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
 * @brief Verifica se la lista di token contiene solo spazi o garbage.
 *
 * @param tokens Lista di token da controllare.
 * @return true se tutti i token sono SPACES o GARBAGE, false altrimenti.
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

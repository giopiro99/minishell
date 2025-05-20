/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sintax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:54:34 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:45:21 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Controlla la presenza di errori di sintassi su un singolo token.
 *
 * Verifica la correttezza sintattica del token passato in input in base al suo tipo.
 * Se il token è una redirezione, un here-document, una pipe o un comando,
 * chiama la funzione specifica di controllo dedicata.
 *
 * @param tmp Il token da verificare.
 * @return true se è stato rilevato un errore di sintassi, false altrimenti.
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
 * @brief Controlla la sintassi di tutta la lista di token.
 *
 * Ignora i token di tipo spazio all'inizio della lista.
 * Se la lista inizia con una pipe, stampa un errore di sintassi.
 * Poi scorre tutti i token e verifica la sintassi tramite ft_sintax_errors.
 *
 * @param tokens Puntatore al primo token della lista.
 * @return true se è stato rilevato un errore di sintassi, false se la sintassi è corretta.
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

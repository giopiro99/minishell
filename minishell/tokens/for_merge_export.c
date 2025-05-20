/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_merge_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:46:19 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:42:35 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <string.h>
#include "tokens.h"

/**
 * @brief Unisce il valore del token corrente con il successivo.
 *
 * Usa `ft_strjoin` per concatenare i valori dei due token e aggiorna
 * il puntatore `to_delete` con il token `next`, che sarà eliminato successivamente.
 *
 * @param to_delete Puntatore al token da eliminare (sarà impostato a `next`)
 * @param next Token successivo da unire
 * @param merged Puntatore alla stringa che contiene il risultato della fusione
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
 * @brief Unisce i token consecutivi di tipo ARGUMENT in un unico token,
 *        per il comando `export`.
 *
 * A partire da un token di tipo ARGUMENT, concatena i token successivi
 * finché sono anch'essi ARGUMENT, aggiornando il valore del token corrente
 * e rimuovendo i successivi dalla lista.
 *
 * @param curr Puntatore al token corrente (inizialmente il primo ARGUMENT dopo "export")
 * @param next Puntatore di lavoro al token successivo
 * @param tokens Puntatore alla testa della lista di token
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
 * @brief Verifica se il comando corrente è `export` e,
 *        se sì, unisce i token di tipo ARGUMENT successivi.
 *
 * Serve a gestire casi in cui `export` riceve un valore con spazi non delimitati da virgolette.
 * Esempio: `export VAR=hello world` → `VAR=hello world` deve essere unito in un unico token.
 *
 * @param tokens Puntatore alla testa della lista di token
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_merge_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:57:45 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:43:41 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Verifica se una stringa è composta solo da caratteri 'n'.
 *
 * Utile per determinare se un'opzione come `-nnn` è valida per `echo -n`.
 *
 * @param str La stringa da controllare
 * @return 1 se contiene solo 'n', altrimenti 0
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
 * @brief Unisce due token concatenando i loro valori.
 *
 * Aggiorna il valore del token `curr` e rimuove il token `next` dalla lista.
 *
 * @param tokens Puntatore alla testa della lista di token
 * @param curr Token corrente da aggiornare
 * @param next Token successivo da unire e rimuovere
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
 * @brief Cerca e unisce tutti i token consecutivi `-n`, `-nn`, etc.
 *        dopo un primo token `-n` valido.
 *
 * Scorre la lista dei token e finché trova token `-n*`, li concatena
 * nel primo `-n` iniziale.
 *
 * @param tokens Puntatore alla testa della lista di token
 * @param curr Token `-n` iniziale
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
 * @brief Controlla se un token rappresenta una valida opzione `-n`
 *        per il comando `echo`.
 *
 * Una valida opzione ha forma `-n`, `-nn`, etc. e tipo ARGUMENT.
 *
 * @param curr Token da verificare
 * @return 1 se è un'opzione `-n` valida, altrimenti 0
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
 * @brief Gestisce la fusione di più token `-n`, `-nn`, ecc. per il comando `echo`.
 *
 * Se il primo token è "echo" e il successivo è un'opzione `-n`, unisce tutte
 * le opzioni `-n*` in un unico token. Questo permette di gestire correttamente
 * sintassi come `echo -n -nn hello`.
 *
 * @param tokens Puntatore alla testa della lista di token
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 11:00:05 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:40:00 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <readline/chardefs.h>
#include <stdbool.h>

/**
 * @brief Assegna i tipi ai token e applica regole specifiche per comandi.
 *
 * Questa funzione elabora la lista di token per:
 * - assegnare tipi (es. ARGUMENT, REDIRECTION, ecc.),
 * - marcare spazi speciali,
 * - unire correttamente i token nei comandi `export` ed `echo`.
 *
 * @param tokens Lista dei token da processare
 */
void	ft_define_tokens(t_token *tokens)
{
	assign_token_types(tokens);
	mark_sign_spaces(tokens);
	ft_merge_tokens_export(&tokens);
	ft_merge_tokens_echo(&tokens);
}

/**
 * @brief Estrae e costruisce un singolo token dall'input utente.
 *
 * In base al carattere corrente, gestisce:
 * - spazi,
 * - virgolette,
 * - caratteri speciali (| < >),
 * - parole semplici.
 *
 * @param input Input da analizzare
 * @param i Puntatore all'indice corrente nell'input
 * @param tokens Puntatore alla lista dei token da riempire
 * @param buffer Buffer temporaneo per la costruzione delle parole
 * @return 0 in caso di successo, -1 in caso di errore
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
 * @brief Tokenizza l'intera stringa di input dell'utente.
 *
 * Alloca un buffer temporaneo, chiama ripetutamente `ft_get_tokens`
 * per costruire la lista di token e infine chiama `ft_define_tokens`
 * per completare l'elaborazione.
 *
 * @param input Input utente da tokenizzare
 * @return Puntatore alla lista dei token generata, o NULL in caso di errore
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

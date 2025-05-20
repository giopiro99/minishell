/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:49:02 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:41:34 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Imposta il tipo di virgolette e il flag corrispondente.
 *
 * Determina se la citazione è singola o doppia e imposta il flag per
 * l'elaborazione successiva.
 *
 * @param quote Puntatore al carattere che conterrà il tipo di virgoletta
 * @param input Stringa di input da analizzare
 * @param i Indice corrente nella stringa
 * @param flag Puntatore al flag da impostare (1 per singole, 2 per doppie)
 */
static void	ft_set_flag(char *quote, char *input, int *i, int *flag)
{
	*quote = input[(*i)++];
	if (*quote == '\'')
		*flag = 1;
	else
		*flag = 2;
}

/**
 * @brief Elabora una stringa racchiusa tra virgolette e la copia nel buffer.
 *
 * Gestisce sia le virgolette singole che doppie.
 *
 * @param input Stringa di input da analizzare
 * @param i Puntatore all'indice corrente nell'input
 * @param j Puntatore all'indice del buffer
 * @param buffer Buffer dove copiare il contenuto tra virgolette
 * @return 1 per virgolette singole, 2 per doppie, -1 in caso di errore
 */
int	ft_for_quotes(char *input, int *i, int *j, char *buffer)
{
	char	quote;
	int		flag;

	quote = 0;
	flag = 0;
	if (input[(*i)] == '\'' || input[(*i)] == '"')
	{
		ft_set_flag(&quote, input, i, &flag);
		while (input[(*i)] && input[(*i)] != quote)
		{
			if (input[(*i)] == quote)
				(*i)++;
			else
				buffer[(*j)++] = input[(*i)++];
		}
		if (input[(*i)] == '\0')
			return (-1);
		(*i)++;
		if (j == 0)
			buffer[(*j)++] = '\0';
	}
	else
		buffer[(*j)++] = input[(*i)++];
	return (flag);
}

/**
 * @brief Gestisce i token racchiusi tra virgolette e li aggiunge alla lista.
 *
 * Chiama `ft_for_quotes`, gestisce eventuali errori di quote non chiuse,
 * e aggiunge il nuovo token alla lista.
 *
 * @param input Stringa di input da analizzare
 * @param i Indice corrente nella stringa
 * @param buffer Buffer temporaneo per costruire il token
 * @param tokens Lista di token a cui aggiungere il risultato
 * @return true in caso di successo, false in caso di errore (quote non chiusa)
 */
bool	handle_quotes(char *input, int *i, char *buffer, t_token **tokens)
{
	int		flag;
	int		j;

	j = 0;
	flag = ft_for_quotes(input, i, &j, buffer);
	if (flag == -1)
	{
		error_ex("minishell: QUOTE ERROR\n");
		free_token(*tokens);
		free(buffer);
		return (false);
	}
	buffer[j] = '\0';
	if (j > 0)
		add_token(tokens, new_token(buffer, get_token_type(buffer, flag, 0)));
	return (true);
}

/**
 * @brief Gestisce i caratteri speciali della shell (|, <, >, <<, >>).
 *
 * Costruisce il token corrispondente e lo aggiunge alla lista dei token.
 *
 * @param input Stringa di input da analizzare
 * @param i Indice corrente nella stringa
 * @param buffer Buffer temporaneo per costruire il token
 * @param tokens Lista di token a cui aggiungere il risultato
 * @return true sempre, non viene gestito nessun errore
 */
bool	handle_special_chars(char *input, int *i,
					char *buffer, t_token **tokens)
{
	int	j;

	j = 0;
	if (input[(*i)] == '|' || input[(*i)] == '<' || input[(*i)] == '>')
	{
		j = 0;
		if ((input[(*i)] == '<' && input[(*i) + 1] == '<')
			|| (input[(*i)] == '>' && input[(*i) + 1] == '>'))
		{
			buffer[j++] = input[(*i)++];
			buffer[j++] = input[(*i)++];
		}
		else
			buffer[j++] = input[(*i)++];
		buffer[j] = '\0';
		add_token(&(*tokens), new_token(buffer,
				get_token_type(buffer, 0, 0)));
	}
	return (true);
}

/**
 * @brief Gestisce la creazione di un token normale (parole, comandi, argomenti).
 *
 * Scorre fino a trovare uno spazio o un carattere speciale.
 *
 * @param input Stringa di input da analizzare
 * @param i Indice corrente nella stringa
 * @param buffer Buffer temporaneo per costruire il token
 * @param tokens Lista di token a cui aggiungere il risultato
 * @return true sempre, non viene gestito nessun errore
 */
bool	handle_word(char *input, int *i, char *buffer, t_token **tokens)
{
	int	j;

	j = 0;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '\'' && input[*i] != '"')
	{
		buffer[j++] = input[(*i)++];
	}
	buffer[j] = '\0';
	if (j > 0)
		add_token(tokens, new_token(buffer, get_token_type(buffer, 0, 0)));
	return (true);
}

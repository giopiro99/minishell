/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_pt2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:04:10 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:12:40 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Verifica se il token rappresenta una redirezione.
 *
 * Considera i tipi REDIR_IN, REDIR_OUT e REDIR_APPEND.
 *
 * @param tokens Puntatore al token da controllare
 * @return true Se il token è una redirezione
 * @return false Altrimenti
 */
bool	is_redir(t_token *tokens)
{
	if (tokens->type == REDIR_IN
		|| tokens->type == REDIR_OUT || tokens->type == REDIR_APPEND)
		return (true);
	return (false);
}

/**
 * @brief Verifica se il token è un argomento valido.
 *
 * Considera ARGUMENT, ENV_VAR, QUOTE e EXIT_STATUS come argomenti.
 *
 * @param tokens Puntatore al token da controllare
 * @return true Se il token è un argomento
 * @return false Altrimenti
 */
bool	is_argument(t_token *tokens)
{
	if (tokens->type == ARGUMENT || tokens->type == ENV_VAR
		|| tokens->type == QUOTE || tokens->type == EXIT_STATUS)
		return (true);
	return (false);
}

/**
 * @brief Verifica se il token rappresenta uno spazio o carattere non valido (garbage).
 *
 * Controlla i tipi SPACES e GARBAGE.
 *
 * @param tokens Puntatore al token da controllare
 * @return true Se il token è uno spazio o garbage
 * @return false Altrimenti
 */
bool	is_space(t_token *tokens)
{
	if (tokens->type == SPACES || tokens->type == GARBAGE)
		return (true);
	return (false);
}


/**
 * @brief Verifica se una stringa è una sequenza valida di '-n' per il comando echo.
 *
 * Controlla che la stringa inizi con "-n" seguito da zero o più 'n'.
 * Es. "-n", "-nn", "-nnn" sono valide, "-nm" non lo è.
 *
 * @param str Stringa da verificare
 * @return true Se è una sequenza valida di '-n'
 * @return false Altrimenti
 */
bool	is_n_sequence(char *str)
{
	int	i;

	i = 2;
	if (!str || str[0] != '-' || str[1] != 'n')
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Assegna il tipo ai token di tipo spazio in base al contesto precedente e successivo.
 *
 * Se il token precedente è un argomento e rappresenta una sequenza '-n', imposta il token corrente come GARBAGE.
 * Se il token precedente è un ARGUMENT, ENV_VAR o QUOTE e il token successivo è uno di questi,
 * e non è già stato trovato uno spazio tra argomenti, imposta il token corrente come SPACES, altrimenti GARBAGE.
 * In tutti gli altri casi imposta GARBAGE.
 *
 * @param prev Token precedente (può essere NULL)
 * @param tokens Token corrente da modificare
 * @param between_arg Puntatore a intero che indica se è stato già trovato uno spazio tra argomenti (0 o 1)
 */
void	ft_assign_spaces(t_token *prev, t_token *tokens, int *between_arg)
{
	if (prev && prev->type == ARGUMENT && is_n_sequence(prev->value))
		tokens->type = GARBAGE;
	else if (prev && ((prev->type == ARGUMENT || prev->type == ENV_VAR
				|| prev->type == QUOTE)
			&& tokens->next && (tokens->next->type == ARGUMENT
				|| tokens->next->type == ENV_VAR
				|| tokens->next->type == QUOTE)))
	{
		if (!*between_arg)
		{
			tokens->type = SPACES;
			*between_arg = 1;
		}
		else
			tokens->type = GARBAGE;
	}
	else
		tokens->type = GARBAGE;
}

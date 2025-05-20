/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_finder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:27:28 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:20:22 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Verifica se una stringa contiene almeno un carattere di doppia virgoletta (").
 *
 * @param str stringa da controllare
 * @return true se la stringa contiene almeno un doppio apice (")
 * @return false altrimenti
 */
bool	ft_find_dquotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			return (true);
		i++;
	}
	return (false);
}

/**
 * @brief Verifica se una stringa contiene almeno un carattere di singola virgoletta (').
 *
 * @param str stringa da controllare
 * @return true se la stringa contiene almeno un singolo apice (')
 * @return false altrimenti
 */
bool	ft_find_squotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}

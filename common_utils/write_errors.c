/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:39:34 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:09:55 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Stampa un messaggio di errore sullo standard error.
 *
 * @param error Puntatore a una stringa contenente il messaggio di errore da stampare.
 */
void	error_ex(const char *error)
{
	ft_printf(2, "%s", error);
}

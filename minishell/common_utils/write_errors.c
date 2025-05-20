/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:39:34 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:01:56 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Prints an error message to standard error.
 *
 * @param error Pointer to a string containing the error message to print.
 */
 void	error_ex(const char *error)
 {
	 ft_printf(2, "%s", error);
 }

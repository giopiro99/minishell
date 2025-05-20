/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_finder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:27:28 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:56:44 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Checks if a string contains at least one double quote character (").
 *
 * @param str string to check
 * @return true if the string contains at least one double quote (")
 * @return false otherwise
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
  * @brief Checks if a string contains at least one single quote character (').
  *
  * @param str string to check
  * @return true if the string contains at least one single quote (')
  * @return false otherwise
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

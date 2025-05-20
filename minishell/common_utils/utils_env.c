/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:06:40 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:58:26 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Checks if the string contains the character '$', indicating an environment variable.
 *
 * @param str string to check
 * @return true if '$' is present, false otherwise
 */
 bool	ft_find_env_var(char *str)
 {
	 int	i;

	 i = 0;
	 while (str[i])
	 {
		 if (str[i] == '$')
			 return (true);
		 i++;
	 }
	 return (false);
 }

 /**
  * @brief Expands an environment variable found in a string.
  *
  * The input is a string `str` and an index `*i` pointing at the '$' character.
  * The function extracts the name of the following variable and replaces it with its value
  * obtained from `env`. The expansion updates the string `*result` by concatenating the found value.
  *
  * @param i pointer to the current index in the string `str`; updated after expansion
  * @param str string containing the variable to expand (e.g., "Hello $USER!")
  * @param env environment structure containing the environment variables
  * @param result pointer to the result string which will be modified (concatenates expanded values)
  */
 void	ft_expand(int *i, char *str, t_env *env, char **result)
 {
	 int		start;
	 char	*var_name;
	 char	*var_value;
	 char	*tmp;

	 (*i)++;
	 start = *i;
	 var_name = NULL;
	 var_value = NULL;
	 if (ft_isdigit(str[(*i)]))
		 (*i)++;
	 else
	 {
		 while (str[(*i)] && (ft_isalnum(str[(*i)]) || str[(*i)] == '_'))
			 (*i)++;
	 }
	 var_name = ft_substr(str, start, *i - start);
	 var_value = ft_get_env_value(env, var_name);
	 if (var_value)
	 {
		 tmp = ft_strjoin(*result, var_value);
		 free (*result);
		 *result = tmp;
	 }
	 free(var_name);
 }

 /**
  * @brief Concatenates a literal part (non-variable) of the string to the result.
  *
  * Copies the substring from `*i` up to the next '$' (or end of string) and concatenates it to `*result`.
  * Updates the index `*i` up to the end of the substring.
  *
  * @param i pointer to the current index in the string `str`
  * @param str input string
  * @param result pointer to the result string
  */
 void	ft_merge_str(int *i, char *str, char **result)
 {
	 int		start;
	 char	*tmp;
	 char	*literal;

	 start = *i;
	 tmp = NULL;
	 literal = NULL;
	 while (str[(*i)] && str[(*i)] != '$')
		 (*i)++;
	 literal = ft_substr(str, start, *i - start);
	 tmp = ft_strjoin(*result, literal);
	 free(*result);
	 *result = tmp;
	 free(literal);
 }

 /**
  * @brief Performs environment variable expansion in a string.
  *
  * Iterates through the string `str`, expanding each environment variable ($VAR) by replacing it
  * with the current value contained in `env`. Returns a new expanded string.
  *
  * @param str string containing possible environment variables to expand
  * @param env environment structure containing variables
  * @return char* new string with all variables expanded
  */
 char	*ft_expand_envvar(char *str, t_env *env)
 {
	 int		i;
	 int		start;
	 char	*result;
	 char	*tmp;
	 char	*literal;

	 i = 0;
	 start = 0;
	 tmp = NULL;
	 literal = NULL;
	 result = ft_strdup("");
	 while (str[i])
	 {
		 if (str[i] == '$')
			 ft_expand(&i, str, env, &result);
		 else
			 ft_merge_str(&i, str, &result);
	 }
	 return (result);
 }

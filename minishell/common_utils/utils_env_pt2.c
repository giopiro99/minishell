/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_pt2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:57:37 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:57:58 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Checks if the token list contains only environment variables, spaces, or garbage tokens.
 *
 * Iterates through the entire `tokens` list. If it encounters a token that is not of type
 * ENV_VAR, SPACES, or GARBAGE, the function returns false; otherwise, it returns true.
 *
 * @param tokens pointer to the first token in the list
 * @return true if all tokens are ENV_VAR, SPACES, or GARBAGE
 * @return false if there is at least one token of a different type
 */
 bool	check_only_env_var(t_token *tokens)
 {
	 t_token	*tmp;

	 tmp = tokens;
	 while (tmp)
	 {
		 if (tmp->type != ENV_VAR && tmp->type != SPACES && tmp->type != GARBAGE)
			 return (false);
		 tmp = tmp->next;
	 }
	 return (true);
 }

 /**
  * @brief Finds the value of an environment variable given its name `key`.
  *
  * Iterates through the `env->env_copy` array, where each element is a "KEY=VALUE" string.
  * If it finds a string that starts with `key` followed by '=', returns a pointer
  * to the value part (after '=').
  *
  * @param env pointer to the environment structure
  * @param key name of the variable to find (without '=')
  * @return char* value of the variable if found, NULL otherwise
  */
 char	*ft_get_env_value(t_env *env, const char *key)
 {
	 int		i;
	 int		key_len;

	 if (!env || !key || !env->env_copy)
		 return (NULL);
	 key_len = ft_strlen(key);
	 i = 0;
	 while (env->env_copy[i])
	 {
		 if (ft_strncmp(env->env_copy[i], key, key_len) == 0
			 && env->env_copy[i][key_len] == '=')
			 return (env->env_copy[i] + key_len + 1);
		 i++;
	 }
	 return (NULL);
 }

 /**
  * @brief Extracts the variable name from a "KEY=VALUE" string.
  *
  * Returns a new string containing only the "KEY" part before the '='.
  *
  * @param env_var string in the form "KEY=VALUE"
  * @return char* new string containing only "KEY"
  */
 char	*get_var_name(const char *env_var)
 {
	 int	i;

	 i = 0;
	 while (env_var[i] != '\0' && env_var[i] != '=')
		 i++;
	 return (ft_substr(env_var, 0, i));
 }

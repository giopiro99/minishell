/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_pt2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:57:37 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:17:45 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Controlla se la lista di token contiene solo variabili d'ambiente, spazi o garbage.
 *
 * Viene attraversata tutta la lista `tokens`. Se si incontra un token che non sia di tipo
 * ENV_VAR, SPACES o GARBAGE, la funzione ritorna false, altrimenti true.
 *
 * @param tokens puntatore al primo token della lista
 * @return true se tutti i token sono ENV_VAR, SPACES o GARBAGE
 * @return false se c'è almeno un token di tipo diverso
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
 * @brief Cerca il valore di una variabile d'ambiente dato il nome `key`.
 *
 * Scorre la matrice `env->env_copy`, ogni elemento è una stringa "KEY=VALUE".
 * Se trova una stringa che inizia con `key` seguita da '=', restituisce il puntatore
 * al valore (parte dopo '=').
 *
 * @param env puntatore alla struttura dell'ambiente
 * @param key nome della variabile da cercare (senza '=')
 * @return char* valore della variabile se trovata, NULL altrimenti
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
 * @brief Estrae il nome di una variabile da una stringa "KEY=VALUE".
 *
 * Restituisce una nuova stringa contenente solo la parte "KEY" prima del '='.
 *
 * @param env_var stringa "KEY=VALUE"
 * @return char* nuova stringa con solo "KEY"
 */
char	*get_var_name(const char *env_var)
{
	int	i;

	i = 0;
	while (env_var[i] != '\0' && env_var[i] != '=')
		i++;
	return (ft_substr(env_var, 0, i));
}

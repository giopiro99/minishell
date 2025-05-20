/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:06:40 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:17:03 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Controlla se la stringa contiene il carattere '$', indicatore di variabile d'ambiente.
 *
 * @param str stringa da controllare
 * @return true se '$' è presente, false altrimenti
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
 * @brief Espande una variabile d'ambiente trovata in una stringa.
 *
 * L'input è una stringa `str` e un indice `*i` che punta sul carattere '$'.
 * La funzione estrae il nome della variabile seguente e la sostituisce con il suo valore
 * ottenuto da `env`. L'espansione aggiorna la stringa `*result` concatenando il valore trovato.
 *
 * @param i puntatore all'indice corrente nella stringa `str`; viene aggiornato alla fine dell'espansione
 * @param str stringa contenente la variabile da espandere (es: "Hello $USER!")
 * @param env struttura contenente le variabili d'ambiente
 * @param result puntatore alla stringa risultato che verrà modificata (concatena valori espansi)
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
 * @brief Concatena una parte letterale (non variabile) della stringa al risultato.
 *
 * Copia la sottostringa da `*i` fino al prossimo '$' (o fine stringa) e la concatena a `*result`.
 * Aggiorna l'indice `*i` fino al termine della sottostringa.
 *
 * @param i puntatore all'indice corrente nella stringa `str`
 * @param str stringa di input
 * @param result puntatore alla stringa risultato
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
 * @brief Esegue l'espansione delle variabili d'ambiente in una stringa.
 *
 * Scorre la stringa `str`, espandendo ogni variabile d'ambiente ($VAR) sostituendola col valore
 * attuale contenuto in `env`. Restituisce una nuova stringa espansa.
 *
 * @param str stringa contenente possibili variabili d'ambiente da espandere
 * @param env struttura contenente variabili d'ambiente
 * @return char* nuova stringa con tutte le variabili espanse
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

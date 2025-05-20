/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:44:40 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:16:09 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Ordina in ordine lessicografico (alfabetico) un array di stringhe (environment variables).
 *
 * Utilizza un algoritmo di bubble sort semplice per ordinare le stringhe puntate da env.
 * La funzione modifica direttamente l'array passato e lo restituisce.
 *
 * @param env array NULL-terminato di stringhe da ordinare
 * @return char** array ordinato (stessa istanza passata in input)
 */
char	**ft_env_sorted(char **env)
{
	int		i;
	int		swapped;
	char	*swap;

	i = 0;
	swap = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (env[i] && env[i + 1])
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				swap = env[i];
				env[i] = env[i + 1];
				env[i + 1] = swap;
				swapped = 1;
			}
			i++;
		}
	}
	return (env);
}

/**
 * @brief Controlla se una stringa è un identificatore valido per una variabile di ambiente.
 *
 * Regole di validità:
 * - Non vuota e non NULL
 * - Il primo carattere deve essere una lettera o '_' oppure ' o " o spazio
 * - Gli altri caratteri fino al '=' devono essere alfanumerici o '_', o ' o " o spazio
 *
 * @param str stringa da validare
 * @return true se valido, false altrimenti
 */
bool	is_valid_identifier(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!str || !str[0])
		return (false);
	while (str[i] && str[i] != '=')
		i++;
	if (!ft_isalpha(str[0]) && str[0] != '_'
		&& str[0] != '\'' && str[0] != '\"' && str[0] != ' ')
		return (false);
	while (j < i)
	{
		if (!ft_isalnum(str[j]) && str[j] != '_' && str[j] != '\''
			&& str[j] != '\"' && str[j] != ' ')
			return (false);
		j++;
	}
	return (true);
}

/**
 * @brief Cerca l'indice di una variabile d'ambiente già presente in env->env_copy.
 *
 * Cerca una variabile con lo stesso nome (parte prima del '=') della stringa args.
 * Restituisce l'indice se trovata, -1 altrimenti.
 *
 * @param env struttura ambiente contenente env_copy (array di stringhe)
 * @param args stringa contenente la variabile da cercare (es: "PATH=/bin")
 * @return int indice se trovata, -1 se non trovata o errori
 */
int	ft_find_export_var(t_env *env, char *args)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	if (!args || !env->env_copy)
		return (-1);
	while (args[len] && args[len] != '=')
		len++;
	while (env->env_copy && env->env_copy[i])
	{
		if (!ft_strncmp(env->env_copy[i], args, len)
			&& env->env_copy[i][len] == '=')
			return (i);
		else if (!ft_strncmp(env->env_copy[i], args, len))
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Aggiunge una nuova variabile all'ambiente (env->env_copy).
 *
 * Alloca un nuovo array con una cella in più, copia l'esistente e aggiunge la nuova variabile
 * in coda, aggiornando env->env_copy.
 *
 * Se la reallocazione fallisce, stampa errore e imposta exit_status.
 *
 * @param env struttura ambiente
 * @param args array di stringhe di variabili da aggiungere (es: ["VAR=val", ...])
 * @param i indice della variabile in args da aggiungere
 */
void	ft_add_var(t_env *env, char **args, int *i)
{
	int		j;
	char	**new_env_copy;

	j = 0;
	new_env_copy = NULL;
	while (env->env_copy[j])
		j++;
	new_env_copy = ft_realloc_mtx(env->env_copy, sizeof(char *) * (j + 2), j);
	if (!new_env_copy)
	{
		ft_printf(2, "Memory allocation failed!\n");
		env->exit_status = 1;
		return ;
	}
	env->env_copy = new_env_copy;
	env->env_copy[j] = ft_strdup(args[(*i)]);
	env->env_copy[j + 1] = NULL;
}

/**
 * @brief Controlla se l'argomento passato a unset è valido.
 *
 * Regole di validità:
 * - Non nullo
 * - Il primo carattere deve essere una lettera
 * - Non deve contenere '='
 * - Non deve contenere caratteri '/', ':'
 * - Tutti i caratteri devono essere alfanumerici o '_'
 *
 * @param arg argomento da controllare
 * @return 1 se valido, 0 altrimenti
 */
int	is_valid_unset_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg || (!ft_isalpha(arg[0])))
		return (0);
	while (arg[i])
	{
		if (arg[i] == '=')
			return (0);
		if (!ft_isalnum(arg[i]) && arg[i] != '_'
			&& arg[i] == '/' && arg[i] == ':')
			return (0);
		i++;
	}
	return (1);
}

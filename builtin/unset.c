/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:49:46 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:02:08 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Copia nell'array new_env tutte le variabili di ambiente tranne quella da eliminare.
 *
 * Scorre l'array env_copy, estrae il nome della variabile da ogni stringa e
 * confronta con arg. Se diverso, duplica la variabile in new_env.
 * In caso di errore di allocazione libera new_env e ritorna.
 *
 * @param env Puntatore alla struttura dell'ambiente shell
 * @param arg Nome della variabile da eliminare
 * @param new_env Array di stringhe in cui copiare le variabili filtrate
 */
static void	ft_for_delete(t_env *env, char *arg, char **new_env)
{
	int		i;
	int		j;
	char	*key;

	i = 0;
	j = 0;
	key = NULL;
	while (env->env_copy[i])
	{
		key = get_var_name(env->env_copy[i]);
		if (!key || ft_strcmp(key, arg) != 0)
		{
			new_env[j] = ft_strdup(env->env_copy[i]);
			if (!new_env[j])
			{
				ft_free_matrix(new_env);
				free(key);
				return ;
			}
			j++;
		}
		free(key);
		i++;
	}
	new_env[j] = NULL;
}

/**
 * @brief Elimina una variabile dall'ambiente.
 *
 * Crea un nuovo array senza la variabile all'indice `index` e aggiorna
 * l'ambiente con il nuovo array, liberando il vecchio.
 *
 * @param env Puntatore alla struttura dell'ambiente shell
 * @param index Indice della variabile da eliminare nell'array env_copy
 * @param arg Nome della variabile da eliminare
 */
void	ft_delete_var(t_env *env, int index, char *arg)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	i = 0;
	new_env = NULL;
	while (env->env_copy[i])
	{
		if (i != index)
			len++;
		i++;
	}
	new_env = malloc(sizeof(char *) * (len + 1));
	if (!new_env)
		return ;
	ft_for_delete(env, arg, new_env);
	ft_free_matrix(env->env_copy);
	env->env_copy = new_env;
}

/**
 * @brief Cerca l'indice di una variabile nell'ambiente.
 *
 * Estrae il nome della variabile da ogni stringa dell'ambiente e confronta con args.
 *
 * @param env Puntatore alla struttura dell'ambiente shell
 * @param args Nome della variabile da cercare
 * @return Indice della variabile trovata, -1 se non presente
 */
int	ft_find_unset_var(t_env *env, char *args)
{
	int		i;
	char	*key;

	i = 0;
	key = NULL;
	while (env->env_copy[i])
	{
		key = get_var_name(env->env_copy[i]);
		if (key && ft_strcmp(key, args) == 0)
		{
			free(key);
			return (i);
		}
		free(key);
		i++;
	}
	return (-1);
}

/**
 * @brief Esegue l'operazione di unset su un singolo argomento.
 *
 * Controlla la validità dell'argomento, cerca la variabile nell'ambiente e
 * la elimina se presente. Imposta exit_status in caso di errore.
 *
 * @param cmd Puntatore al comando in esecuzione
 * @param env Puntatore alla struttura dell'ambiente shell
 * @param i Indice corrente degli argomenti (incrementato alla fine)
 * @param index Indice trovato della variabile nell'ambiente
 * @return true se l'argomento è valido, false altrimenti
 */
bool	exe_unset(t_cmd *cmd, t_env *env, int *i, int index)
{
	if (is_valid_unset_arg(cmd->args[(*i)]))
	{
		index = ft_find_unset_var(env, cmd->args[(*i)]);
		if (index >= 0)
			ft_delete_var(env, index, cmd->args[(*i)]);
	}
	else
	{
		ft_printf(2, "unset: `%s': not a valid identifier\n", cmd->args[(*i)]);
		env->exit_status = 1;
		return (false);
	}
	(*i)++;
	return (true);
}

/**
 * @brief Implementa il builtin unset.
 *
 * Per ogni argomento valido del comando, elimina la variabile dall'ambiente.
 * Gestisce l'input/output e ripristina gli stati se necessario.
 *
 * @param cmd Puntatore al comando unset
 * @param env Puntatore alla struttura dell'ambiente shell
 * @param head Puntatore alla lista delle pipe (per la gestione della memoria)
 */
void	ft_unset(t_cmd *cmd, t_env *env, t_pipe *head)
{
	int	index;
	int	i;
	int	fd;
	int	saved_stdin;

	i = 0;
	index = -1;
	saved_stdin = 0;
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return ;
	while (cmd->args[i])
	{
		if (!exe_unset(cmd, env, &i, index))
			return ;
	}
	if (cmd->input)
		restore_sdin(&saved_stdin);
	if (fd != 1)
		close (fd);
	env->exit_status = 0;
	if (cmd->pipe_flag)
		free_all_pipe(head, env);
}

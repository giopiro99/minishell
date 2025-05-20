/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:21:12 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:58:11 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdbool.h>

/**
 * @brief Esegue export su un singolo argomento.
 *
 * Verifica se l'identificatore è valido. Se non lo è, stampa errore e setta exit_status.
 * Se la variabile esiste già nell'ambiente, la aggiorna solo se contiene un '=' (nuovo valore).
 * Altrimenti aggiunge una nuova variabile all'ambiente.
 *
 * @param args  Array di stringhe degli argomenti di export
 * @param i     Indice corrente nell'array args, passato per riferimento
 * @param env   Puntatore alla struttura ambiente shell
 */
void	ft_exe_export(char **args, int *i, t_env *env)
{
	int		index;

	index = -1;
	if (ft_isspace(args[(*i)][0]) && args[(*i)][1] == '\0')
		(*i)++;
	if (!is_valid_identifier(args[(*i)]))
	{
		ft_printf(2, "export: %s not a valid identifier\n", args[(*i)]);
		env->exit_status = 1;
	}
	else
	{
		index = ft_find_export_var(env, args[(*i)]);
		if (index >= 0)
		{
			if (strchr(args[(*i)], '='))
			{
				free(env->env_copy[index]);
				env->env_copy[index] = ft_strdup(args[(*i)]);
			}
		}
		else
			ft_add_var(env, args, i);
	}
}

/**
 * @brief Funzione helper che stampa una singola variabile exportata
 *        con formato "declare -x VAR="value"" se la variabile ha valore.
 *
 * @param env_sorted  Array di stringhe contenente variabili d'ambiente ordinate
 * @param fd          File descriptor su cui scrivere (stdout o file)
 * @param i           Indice corrente nell'array env_sorted, passato per riferimento
 */
static void	ft_too_write(char **env_sorted, int fd, int *i)
{
	int		j;
	int		val_flag;
	int		equals_flag;

	j = 0;
	val_flag = 0;
	equals_flag = 0;
	write(fd, "declare -x ", 11);
	while (env_sorted[*i][j])
	{
		write(fd, &env_sorted[*i][j], 1);
		if (env_sorted[*i][j] == '=' && !equals_flag)
		{
			val_flag = 1;
			equals_flag = 1;
			write(fd, "\"", 1);
		}
		j++;
	}
	if (val_flag == 1)
		write(fd, "\"", 1);
	write(fd, "\n", 1);
}

/**
 * @brief Gestisce il comando export senza argomenti.
 *
 * Stampa tutte le variabili d'ambiente ordinate, usando il formato di export.
 *
 * @param env  Puntatore alla struttura ambiente shell
 * @param fd   File descriptor su cui scrivere (stdout o file)
 */
static void	ft_export_no_args(t_env *env, int fd)
{
	char	**env_sorted;
	int		i;

	i = 0;
	env_sorted = ft_env_sorted(env->env_copy);
	while (env_sorted[i])
	{
		ft_too_write(env_sorted, fd, &i);
		i++;
	}
}

/**
 * @brief Gestisce il comando export con argomenti.
 *
 * Per ogni argomento valido chiama ft_exe_export per aggiungere/aggiornare
 * le variabili d'ambiente.
 *
 * @param cmd  Puntatore alla struttura comando contenente gli argomenti
 * @param env  Puntatore alla struttura ambiente shell
 */
static void	ft_export_with_args(t_cmd *cmd, t_env *env)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i] && *cmd->args[i] != '\0')
			ft_exe_export(cmd->args, &i, env);
		i++;
	}
	env->exit_status = 0;
}

/**
 * @brief Funzione principale di gestione del builtin export.
 *
 * Gestisce redirezioni input/output, stampa o modifica variabili d'ambiente,
 * chiude eventuali file descriptor, ripristina stdin e gestisce la memoria pipe.
 *
 * @param cmd   Puntatore alla struttura comando con argomenti e flags
 * @param env   Puntatore alla struttura ambiente shell
 * @param head  Puntatore alla lista pipe per eventuale pulizia memoria
 */
void	ft_export(t_cmd *cmd, t_env *env, t_pipe *head)
{
	int		fd;
	int		saved_stdin;

	saved_stdin = 0;
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return ;
	if (!cmd->args[0])
		ft_export_no_args(env, fd);
	else
		ft_export_with_args(cmd, env);
	if (fd != 1)
		close(fd);
	if (cmd->input)
		restore_sdin(&saved_stdin);
	env->exit_status = 0;
	if (cmd->pipe_flag)
		free_all_pipe(head, env);
}

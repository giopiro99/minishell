/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:06:23 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:00:14 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Verifica se una stringa di ambiente contiene un valore dopo '='.
 *
 * Scorre la stringa fino al carattere '=' e controlla se esiste un valore non vuoto dopo.
 *
 * @param str Stringa da controllare (es. "KEY=VALUE")
 * @return int 1 se c'è un valore dopo '=', 0 altrimenti
 */
static int	check_value(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] != '=')
		i++;
	if (str[i + 1] != '\0')
		return (1);
	return (0);
}

/**
 * @brief Stampa le variabili d'ambiente valide sul file descriptor specificato.
 *
 * Scorre l'array env_copy e stampa solo le variabili che contengono '=' e un valore valido.
 * Imposta lo stato di uscita a 0.
 *
 * @param env Puntatore alla struttura ambiente shell
 * @param i   Puntatore all'indice da cui iniziare a scorrere env_copy
 * @param fd  File descriptor dove stampare (stdout o file)
 */
static void	print_args(t_env *env, int *i, int fd)
{
	while (env->env_copy[(*i)])
	{
		if (strchr(env->env_copy[(*i)], '=')
			&& check_value(env->env_copy[(*i)]))
			ft_printf(fd, "%s\n", env->env_copy[(*i)]);
		(*i)++;
	}
	env->exit_status = 0;
}

/**
 * @brief Gestisce errori di argomenti non validi passati al comando env.
 *
 * Se è presente un argomento, stampa un messaggio di errore e setta
 * exit_status a 1, segnando l'errore tramite il flag passato per riferimento.
 *
 * @param env Puntatore alla struttura ambiente shell
 * @param cmd Puntatore alla struttura comando
 * @param i   Puntatore all'indice dell'argomento corrente
 * @param error_flag Puntatore a flag di errore da settare a 1 in caso di errore
 */
static void	ft_env_error(t_env *env, t_cmd *cmd, int *i, int *error_flag)
{
	if (cmd->args[*i])
	{
		ft_printf(2, "env: %s not a valid identifier\n", cmd->args[*i]);
		env->exit_status = 1;
		*error_flag = 1;
	}
}

/**
 * @brief Implementazione del comando builtin env.
 *
 * Gestisce la redirezione degli I/O, verifica la validità degli argomenti,
 * stampa le variabili d'ambiente con valori validi e pulisce le risorse.
 *
 * @param cmd  Puntatore alla struttura comando con argomenti e flags
 * @param env  Puntatore alla struttura ambiente shell
 * @param head Puntatore alla lista pipe per la gestione della memoria in caso di pipe
 */
void	ft_env(t_cmd *cmd, t_env *env, t_pipe *head)
{
	int	i;
	int	fd;
	int	saved_stdin;
	int	error_flag;

	i = 0;
	error_flag = 0;
	saved_stdin = 0;
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return ;
	ft_env_error(env, cmd, &i, &error_flag);
	if (!error_flag)
		print_args(env, &i, fd);
	if (fd != 1)
		close(fd);
	if (cmd->input)
		restore_sdin(&saved_stdin);
	if (cmd->pipe_flag)
		free_all_pipe(head, env);
	return ;
}

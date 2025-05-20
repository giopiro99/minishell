/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:08:46 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:59:18 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Gestisce il caso in cui exit viene chiamato senza argomenti oppure con "0".
 *
 * Se non ci sono argomenti, stampa "exit", libera risorse e termina con lo
 * stato di uscita corrente di env->exit_status.
 * Se l'argomento è esattamente "0", stampa "exit", libera risorse, setta exit_status a 1
 * e termina con codice 0.
 *
 * @param env  Puntatore all'ambiente shell
 * @param fd   File descriptor per output (stdout o file)
 * @param cmd  Puntatore alla struttura comando con argomenti
 */
static	void	ft_exit_no_args_or_0(t_env *env, int fd, t_cmd *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (!cmd->args[0])
	{
		exit_status = env->exit_status;
		if (fd != 1)
			close(fd);
		return (free_all(cmd, env), ft_printf(1, "exit\n"), exit(exit_status));
	}
	else if (!ft_strcmp(cmd->args[0], "0"))
	{
		if (fd != 1)
			close (fd);
		env->exit_status = 1;
		free_all(cmd, env);
		ft_printf(1, "exit\n");
		exit (0);
	}
}

/**
 * @brief Controlla se ci sono troppi argomenti per il comando exit.
 *
 * Se ci sono più di un argomento, stampa errore "too many arguments",
 * libera risorse e ritorna 1 per indicare errore.
 * Altrimenti ritorna 0.
 *
 * @param cmd   Puntatore alla struttura comando
 * @param fd    File descriptor per output
 * @param head  Puntatore alla lista pipe per gestione memoria
 * @param env   Puntatore all'ambiente shell
 * @return int  1 se errore, 0 altrimenti
 */
static int	ft_exit_too_args(t_cmd *cmd, int fd, t_pipe *head, t_env *env)
{
	if (cmd->args[1])
	{
		if (fd != 1)
			close (fd);
		if (cmd->pipe_flag)
			free_all_pipe(head, env);
		ft_printf(1, "minishell: exit: too many arguments\n");
		return (1);
	}
	return (0);
}

/**
 * @brief Gestisce errori relativi all'argomento di exit non numerico o nullo.
 *
 * Se exit_n è zero (impossibile convertire l'argomento), stampa
 * "exit" e "exit: numeric argument required", libera risorse e termina.
 *
 * @param exit_n Valore convertito dell'argomento di exit
 * @param cmd    Puntatore alla struttura comando
 * @param env    Puntatore all'ambiente shell
 * @param head   Puntatore alla lista pipe
 */
static void	ft_error_exit(int exit_n, t_cmd *cmd, t_env *env, t_pipe *head)
{
	if (!exit_n)
	{
		if (!cmd->pipe_flag)
			free_all(cmd, env);
		else
			free_all_pipe(head, env);
		return (ft_printf(2, "exit\nexit: numeric argument required\n")
			, exit(0));
	}
}

/**
 * @brief Implementazione del comando builtin exit.
 *
 * Gestisce redirezioni, verifica argomenti, gestisce casi particolari come nessun argomento,
 * argomento "0", troppi argomenti o argomenti non numerici.
 * Alla fine libera risorse, stampa "exit" e termina il processo con il codice corretto.
 *
 * @param cmd   Puntatore alla struttura comando con argomenti e flags
 * @param env   Puntatore all'ambiente shell
 * @param head  Puntatore alla lista pipe per eventuale pulizia memoria
 */
void	ft_exit(t_cmd *cmd, t_env *env, t_pipe *head)
{
	long	exit_n;
	int		fd;

	exit_n = 0;
	fd = check_value_fd(cmd->append, cmd->output, env);
	if (ft_check_fd(fd, env) == -1 || cmd->open_error)
		return ;
	ft_exit_no_args_or_0(env, fd, cmd);
	exit_n = ft_atol(cmd->args[0]);
	ft_error_exit(exit_n, cmd, env, head);
	if (ft_exit_too_args(cmd, fd, head, env))
		return ;
	if (exit_n > 255 || exit_n < 0)
		exit_n = exit_n % 256;
	env->exit_status = exit_n;
	if (!cmd->pipe_flag)
		free_all(cmd, env);
	else
		free_all_pipe(head, env);
	ft_printf(1, "exit\n");
	if (fd != 1)
		close (fd);
	exit(exit_n);
}

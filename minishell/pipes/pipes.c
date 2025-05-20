/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:41 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:07:19 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief Esegue un comando di sistema all'interno di una pipe.
 *
 * Cerca il path dell'eseguibile, prepara la matrice degli argomenti,
 * gestisce eventuali redirezioni, quindi esegue il comando tramite execve.
 *
 * @param env Puntatore alla struttura ambiente shell
 * @param cmd Puntatore alla struttura comando da eseguire
 * @return int 0 se successo, -1 in caso di errore, 127 se comando non trovato
 */
int	ft_exe_syscmd_in_pipe(t_env *env, t_cmd *cmd)
{
	char	*dir;
	int		fd;
	int		saved_stdin;
	char	**matrix_exe;
	int		i;

	i = 0;
	matrix_exe = NULL;
	fd = 0;
	saved_stdin = 0;
	dir = ft_path_finder(env, cmd);
	if (!dir)
		return (env->exit_status = 127, ft_printf(2, "Command not found\n"));
	matrix_exe = ft_mtx_cmd(cmd);
	if (!matrix_exe)
		return (env->exit_status = -1, ft_printf(2, "Malloc Error\n"));
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return (env->exit_status = -1, ft_free_matrix(matrix_exe),
			free(dir), -1);
	ft_execution_execve(&fd, dir, matrix_exe, env);
	return (0);
}

/**
 * @brief Routine eseguita dal processo figlio per ogni comando in pipe.
 *
 * Gestisce la duplicazione dei file descriptor per input/output in base alla posizione
 * nella pipe, chiude i descrittori non necessari e chiama l'esecuzione del comando.
 *
 * @param prev_fd  File descriptor del lato di lettura della pipe precedente (-1 se nessuna)
 * @param current  Puntatore al nodo corrente della pipe
 * @param head_pipe Puntatore alla testa della lista pipe (per gestione memoria)
 * @param env     Puntatore alla struttura ambiente shell
 */
void	ft_child_routine(int *prev_fd, t_pipe *current,
		t_pipe *head_pipe, t_env *env)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (*prev_fd != -1)
	{
		if (dup2(*prev_fd, STDIN_FILENO) == -1)
			return ;
		close(*prev_fd);
	}
	if (current->next || current->one_cmd_flag)
	{
		close(current->pipefd[0]);
		if (dup2(current->pipefd[1], STDOUT_FILENO) == -1)
			return ;
		close (current->pipefd[1]);
	}
	else
	{
		if (current->pipefd[0] != -1)
			close(current->pipefd[0]);
		if (current->pipefd[1] != -1)
			close(current->pipefd[1]);
	}
	ft_exe_pipe_cmd(current, env, current->cmd->tokens, head_pipe);
	return ;
}


/**
 * @brief Routine eseguita dal processo padre dopo la fork di un processo figlio.
 *
 * Chiude file descriptor non più necessari e prepara il prev_fd per il prossimo ciclo.
 *
 * @param prev_fd  Puntatore al file descriptor della pipe precedente (lato lettura)
 * @param current  Puntatore al nodo pipe corrente
 */
void	ft_parent_routine(int *prev_fd, t_pipe *current)
{
	if (*prev_fd != -1)
		close (*prev_fd);
	if (current->next || current->one_cmd_flag)
	{
		close(current->pipefd[1]);
		*prev_fd = current->pipefd[0];
	}
}

/**
 * @brief Gestisce la creazione della pipe e la fork per eseguire il comando corrente.
 *
 * Crea la pipe se necessario, esegue la fork e chiama la routine del figlio o del padre.
 *
 * @param current    Puntatore al nodo pipe corrente
 * @param env        Puntatore alla struttura ambiente shell
 * @param head_pipe  Puntatore alla testa della lista pipe
 * @param prev_fd    Puntatore al file descriptor per input del prossimo comando
 */
static void	ft_pipe_routine(t_pipe *current, t_env *env,
		t_pipe *head_pipe, int *prev_fd)
{
	if (current->next || current->one_cmd_flag)
		pipe(current->pipefd);
	current->pid = fork();
	if (current->pid == 0)
	{
		ft_child_routine(prev_fd, current, head_pipe, env);
		if (*prev_fd != -1)
			close (*prev_fd);
		exit(0);
	}
	else
		ft_parent_routine(prev_fd, current);
}

/**
 * @brief Esegue una lista di comandi collegati tramite pipe.
 *
 * Per ogni comando crea pipe e fork, collega gli input/output e attende la terminazione
 * di tutti i processi figli aggiornando lo stato di uscita.
 *
 * @param head_pipe Puntatore alla testa della lista pipe contenente i comandi
 * @param env       Puntatore alla struttura ambiente shell
 */
void	execute_pipe(t_pipe *head_pipe, t_env *env)
{
	t_pipe	*current;
	int		prev_fd;
	int		status;

	prev_fd = -1;
	current = head_pipe;
	status = 0;
	if (!current->next)
		current->one_cmd_flag = true;
	while (current)
	{
		ft_pipe_routine(current, env, head_pipe, &prev_fd);
		current = current->next;
	}
	while (waitpid(-1, &status, 0) > 0)
		get_exit_status(status, env);
	if (prev_fd != -1)
		close (prev_fd);
}

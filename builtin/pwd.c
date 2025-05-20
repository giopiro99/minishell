/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:32:58 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:01:53 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>

/**
 * @brief Chiude il file descriptor se diverso da 1 e ripristina lo stdin se necessario.
 *
 * Se il file descriptor passato non è lo standard output (1),
 * lo chiude. Se il comando ha un input ridiretto, ripristina lo stdin
 * al suo valore salvato.
 *
 * @param fd Puntatore al file descriptor da chiudere
 * @param cmd Puntatore alla struttura comando
 * @param saved_stdin Puntatore alla variabile che contiene il valore originale di stdin
 */
static void	ft_too_close(int *fd, t_cmd *cmd, int *saved_stdin)
{
	if (*fd != 1)
		close (*fd);
	if (cmd->input)
		restore_sdin(saved_stdin);
}

/**
 * @brief Implementa il comando builtin pwd.
 *
 * Ottiene la directory di lavoro corrente e la stampa sul file descriptor
 * specificato dal comando (output ridiretto se presente).
 * Gestisce errori di getcwd e ripristina lo stato degli input/output.
 * Aggiorna lo stato di uscita dell'ambiente.
 *
 * @param cmd Puntatore alla struttura comando corrente
 * @param env Puntatore alla struttura ambiente shell
 * @param head Puntatore alla lista di pipe, usato per liberare memoria in caso di pipe
 */
void	ft_pwd(t_cmd *cmd, t_env *env, t_pipe *head)
{
	char	*pwd;
	int		fd;
	int		saved_stdin;

	saved_stdin = 0;
	pwd = NULL;
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		env->exit_status = 1;
		perror("pwd");
		return ;
	}
	ft_printf(fd, "%s\n", pwd);
	free(pwd);
	ft_too_close(&fd, cmd, &saved_stdin);
	env->exit_status = 0;
	if (cmd->pipe_flag)
		free_all_pipe(head, env);
	return ;
}

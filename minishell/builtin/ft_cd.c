/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:32:22 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:54:56 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>

/**
 * @brief Imposta o aggiorna una variabile d'ambiente nell'array env_copy.
 *
 * Cerca una variabile con il nome key e la aggiorna con var.
 * Se la variabile non esiste, non la aggiunge (solo aggiornamento).
 *
 * @param key Nome della variabile d'ambiente
 * @param var Valore della variabile d'ambiente
 * @param env Puntatore alla struttura ambiente
 */
void	ft_setenv(const char *key, const char *var, t_env *env)
{
	int		i;
	char	*new_var;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return ;
	new_var = ft_strjoin(tmp, var);
	free(tmp);
	if (!new_var)
		return ;
	while (env->env_copy && env->env_copy[i])
	{
		if (ft_strncmp(env->env_copy[i], key, ft_strlen(key)) == 0
			&& (env->env_copy[i][ft_strlen(key)] == '='))
		{
			free(env->env_copy[i]);
			env->env_copy[i] = new_var;
			return ;
		}
		i++;
	}
}

/**
 * @brief Elabora gli argomenti passati al comando cd per ricavare il percorso.
 *
 * Se non sono passati argomenti usa la variabile HOME,
 * se ci sono più argomenti restituisce errore.
 *
 * @param cmd Puntatore alla struttura comando
 * @param path Puntatore al puntatore di destinazione per il percorso ricavato
 * @return 0 se successo, -1 o -2 in caso di errore
 */
int	ft_exe_cd(t_cmd *cmd, t_env *env, char **path)
{
	if (!cmd->args[0])
		*path = ft_strdup(ft_get_env_value(env, "HOME"));
	else if (cmd->args[1])
		return (env->exit_status = 1,
			ft_printf(2, "cd: too many arguments\n"), -1);
	else
		*path = ft_strdup(cmd->args[0]);
	if (!path)
		return (env->exit_status = 1, perror("cd"), -2);
	return (0);
}

/**
 * @brief Cambia la directory corrente e aggiorna le variabili PWD e OLDPWD.
 *
 * Effettua il cambio directory con chdir, aggiorna OLDPWD e PWD nell'ambiente.
 *
 * @param env Puntatore alla struttura ambiente
 * @param path Percorso della directory in cui spostarsi
 * @return 0 se successo, -1 in caso di errore
 */
int	ft_set_env(t_env *env, char *path)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = ft_strdup(ft_get_env_value(env, "PWD"));
	if (!old_pwd)
		old_pwd = getcwd(NULL, 0);
	if (chdir(path))
		return (env->exit_status = 1, free(old_pwd),
			free(path), perror("cd"), -1);
	new_pwd = getcwd(NULL, 0);
	if (old_pwd)
		ft_setenv("OLDPWD", old_pwd, env);
	if (new_pwd)
		ft_setenv("PWD", new_pwd, env);
	free(old_pwd);
	free(new_pwd);
	free(path);
	return (0);
}

/**
 * @brief Ripristina l'input standard e chiude il file descriptor se necessario.
 *
 * @param cmd Puntatore alla struttura comando
 * @param saved_stdin Puntatore all'intero che contiene il file descriptor salvato per stdin
 * @param fd Puntatore al file descriptor da chiudere
 */
void	ft_clean_exit(t_cmd *cmd, int *saved_stdin, int *fd)
{
	if (cmd->input)
		restore_sdin(saved_stdin);
	if (*fd != 1)
		close(*fd);
}

/**
 * @brief Implementazione del comando builtin cd.
 *
 * Gestisce l'input/output ridiretto, verifica argomenti, esegue cambio directory,
 * aggiorna variabili d'ambiente, e gestisce la pulizia finale.
 *
 * @param cmd Puntatore alla struttura comando corrente
 * @param env Puntatore alla struttura ambiente shell
 * @param head Puntatore alla lista di pipe per la gestione della memoria
 * @return 0 se successo, -1 in caso di errore
 */
int	ft_cd(t_cmd *cmd, t_env *env, t_pipe *head)
{
	char	*path;
	int		fd;
	int		saved_stdin;

	path = NULL;
	saved_stdin = 0;
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1 || cmd->open_error)
		return (-1);
	if (ft_exe_cd(cmd, env, &path) == -1)
	{
		ft_clean_exit(cmd, &saved_stdin, &fd);
		return (-1);
	}
	else if (!path)
		return (-1);
	if (ft_set_env(env, path) == -1)
		return (-1);
	ft_clean_exit(cmd, &saved_stdin, &fd);
	env->exit_status = 0;
	if (cmd->pipe_flag)
		free_all_pipe(head, env);
	return (0);
}

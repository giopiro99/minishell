/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:05:25 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:05:37 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "system_commands.h"

/**
 * @brief Cerca il percorso eseguibile di un comando.
 *
 * Se il comando è un percorso relativo o assoluto, verifica i permessi di esecuzione.
 * Altrimenti, cerca il comando in tutte le directory indicate nella variabile PATH.
 * Restituisce una stringa allocata con il percorso completo o NULL se non trovato.
 *
 * @param env Lista delle variabili d’ambiente
 * @param cmd Struttura del comando contenente nome e argomenti
 * @return char* Percorso completo del comando o NULL se non trovato
 */
char	*ft_path_finder(t_env *env, t_cmd *cmd)
{
	char	*path;
	char	**dir_matrix;
	char	*cmd_path;
	int		i;

	i = 0;
	if (cmd->cmd[0] == '.' || cmd->cmd[0] == '/' || (cmd->cmd[0] == '.' && cmd->cmd[1] == '/'))
	{
		if ((cmd->cmd[0] == '.' && !cmd->cmd[1]) || (cmd->cmd[0] == '.' && cmd->cmd[1] == '/' && !cmd->cmd[2]))
			return (NULL);
		if (access(cmd->cmd, X_OK) == 0)
			return (ft_strdup(cmd->cmd));
		return (NULL);
	}
	path = ft_strdup(ft_get_env_value(env, "PATH"));
	dir_matrix = ft_split(path, ':');
	while (dir_matrix[i++] != NULL)
	{
		cmd_path = ft_strjoin_three(dir_matrix[i], "/", cmd->cmd);
		if (cmd_path && access(cmd_path, X_OK) == 0)
			return (ft_free_matrix(dir_matrix), free(path), cmd_path);
		else
			free(cmd_path);
	}
	return (ft_free_matrix(dir_matrix), free(path), NULL);
}

/**
 * @brief Crea una matrice di argomenti per l’esecuzione di un comando.
 *
 * Prepara una matrice di stringhe con il comando e i suoi argomenti, terminata da NULL,
 * adatta per la chiamata execve.
 *
 * @param cmd Struttura del comando contenente nome e argomenti
 * @return char** Matrice di stringhe per execve o NULL in caso di errore di allocazione
 */
char	**ft_mtx_cmd(t_cmd *cmd)
{
	char	**matrix_exe;
	int		i;
	int		j;

	i = 0;
	j = 1;
	matrix_exe = malloc(sizeof(char *) * (ft_matrix_len(cmd->args) + 2));
	if (!matrix_exe)
		return (ft_printf(2, "Malloc Error\n"), NULL);
	matrix_exe[0] = ft_strdup(cmd->cmd);
	while (cmd->args[i])
	{
		matrix_exe[j] = ft_strdup(cmd->args[i]);
		i++;
		j++;
	}
	matrix_exe[j] = NULL;
	return (matrix_exe);
}

/**
 * @brief Esegue un comando esterno con `execve`, reindirizzando stdout se necessario.
 *
 * Se `dir` è valido, chiama `execve` con la matrice di argomenti e l’ambiente passato.
 * In caso di errore stampa il messaggio e imposta `exit_status` a 126.
 *
 * @param dir Percorso assoluto del binario da eseguire
 * @param matrix_exe Matrice di argomenti per execve
 * @param env Ambiente della shell
 */
void	ft_execution(char **dir, char **matrix_exe, t_env *env)
{
	if (dir && execve(*dir, matrix_exe, env->env_copy) == -1)
	{
		perror("execve");
		ft_free_matrix(matrix_exe);
		free(dir);
		env->exit_status = 126;
	}
}

/**
 * @brief Funzione eseguita dal processo figlio per eseguire un comando.
 *
 * Prepara gli argomenti, gestisce i file descriptor di output,
 * imposta i segnali di default per SIGINT e SIGQUIT, e chiama ft_execution.
 * Restituisce -1 in caso di errore o 0 altrimenti.
 *
 * @param cmd Struttura del comando
 * @param env Ambiente della shell
 * @param dir Puntatore al percorso del comando
 * @return int 0 se successo, -1 se errore
 */
int	ft_child(t_cmd *cmd, t_env *env, char **dir)
{
	char	**matrix_exe;
	int		saved_stdin;
	int		fd;

	saved_stdin = 0;
	matrix_exe = ft_mtx_cmd(cmd);
	fd = 1;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (!matrix_exe)
		return (-1);
	fd = ft_for_fd(cmd, env, &saved_stdin);
	if (fd == -1)
		return (free(*dir),
			ft_free_matrix(matrix_exe), free_all(cmd, env), -1);
	if (fd != 1)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), -1);
		close (fd);
	}
	ft_execution(dir, matrix_exe, env);
	return (0);
}

/**
 * @brief Esegue un comando di sistema creando un processo figlio.
 *
 * Cerca il percorso del comando, crea un processo figlio,
 * esegue il comando tramite ft_child, e attende la terminazione.
 * Aggiorna lo stato di uscita della shell.
 *
 * @param env Ambiente della shell
 * @param cmd Struttura del comando
 * @return int 0 se successo, -1 o codice errore altrimenti
 */
int	ft_exe_syscmd(t_env *env, t_cmd *cmd)
{
	pid_t	pid;
	char	*dir;
	int		i;
	int		status;

	if (cmd->open_error)
		return (-1);
	i = 0;
	dir = ft_path_finder(env, cmd);
	if (!dir)
		return (env->exit_status = 127, ft_printf(2,
				"minishell: command not found: %s\n", cmd->cmd));
	pid = fork();
	if (pid == 0)
	{
		ft_child(cmd, env, &dir);
		exit(126);
	}
	else
	{
		waitpid(pid, &status, 0);
		get_exit_status(status, env);
	}
	return (free(dir), 0);
}

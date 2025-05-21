/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:05:25 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:29:34 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "system_commands.h"

#include <stdbool.h>

/**
 * @brief Checks if a command is composed only of '.' and '/'.
 *
 * This helps to detect invalid commands like ".", "./", "././", etc.,
 * which are not executable files but directories or incomplete paths.
 *
 * @param str The command string to check
 * @return true If the command is made only of '.' and '/'
 * @return false Otherwise
 */
bool	ft_is_only_dot_slash(const char *str)
{
	int	i;

	if (!str)
		return (true);
	i = 0;
	while (str[i])
	{
		if (str[i] != '.' && str[i] != '/')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Searches for the executable path of a command.
 *
 * If the command is a relative or absolute path, checks for execution permissions.
 * Otherwise, searches the command in all directories listed in the PATH environment variable.
 * Returns a newly allocated string with the full path or NULL if not found.
 *
 * @param env List of environment variables
 * @param cmd Command structure containing the name and arguments
 * @return char* Full path to the command executable or NULL if not found
 */
char	*ft_path_finder(t_env *env, t_cmd *cmd)
{
	char	*path;
	char	**dir_matrix;
	char	*cmd_path;
	int		i;

	i = 0;
	if (cmd->cmd[0] == '.' || cmd->cmd[0] == '/')
	{
		if (ft_is_only_dot_slash(cmd->cmd))
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
 * @brief Creates an argument matrix for command execution.
 *
 * Prepares a null-terminated array of strings containing the command and its arguments,
 * suitable for passing to execve.
 *
 * @param cmd Command structure containing name and arguments
 * @return char** Null-terminated array of strings for execve or NULL on allocation failure
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
 * @brief Executes an external command with execve, redirecting stdout if needed.
 *
 * If dir is valid, calls execve with the argument matrix and the environment.
 * On error, prints the error message and sets exit_status to 126.
 *
 * @param dir Absolute path to the executable binary
 * @param matrix_exe Argument matrix for execve
 * @param env Shell environment
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
 * @brief Function executed by the child process to run a command.
 *
 * Prepares arguments, manages output file descriptors,
 * sets default signal handlers for SIGINT and SIGQUIT,
 * and calls ft_execution.
 * Returns -1 on error, 0 otherwise.
 *
 * @param cmd Command structure
 * @param env Shell environment
 * @param dir Pointer to the command path string
 * @return int 0 on success, -1 on error
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
 * @brief Executes a system command by creating a child process.
 *
 * Searches the command path, forks a child process,
 * runs the command via ft_child, and waits for its termination.
 * Updates the shell exit status accordingly.
 *
 * @param env Shell environment
 * @param cmd Command structure
 * @return int 0 on success, -1 or error code otherwise
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

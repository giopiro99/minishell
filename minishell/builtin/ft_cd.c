/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:32:22 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:48:25 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Sets or updates an environment variable in the env_copy array.
 *
 * Searches for a variable with the name key and updates it with var.
 * If the variable doesn't exist, it does not add it (update only).
 *
 * @param key Name of the environment variable
 * @param var Value of the environment variable
 * @param env Pointer to the environment structure
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
  * @brief Processes the arguments passed to the cd command to determine the path.
  *
  * Uses the HOME variable if no arguments are passed.
  * Returns error if there are too many arguments.
  *
  * @param cmd Pointer to the command structure
  * @param path Pointer to the pointer to store the resulting path
  * @return 0 on success, -1 or -2 on error
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
  * @brief Changes the current directory and updates PWD and OLDPWD environment variables.
  *
  * Performs chdir, updates OLDPWD and PWD in the environment.
  *
  * @param env Pointer to the environment structure
  * @param path Directory path to change to
  * @return 0 on success, -1 on failure
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
  * @brief Restores standard input and closes file descriptor if needed.
  *
  * @param cmd Pointer to the command structure
  * @param saved_stdin Pointer to saved stdin file descriptor
  * @param fd Pointer to the file descriptor to close
  */
 void	ft_clean_exit(t_cmd *cmd, int *saved_stdin, int *fd)
 {
	 if (cmd->input)
		 restore_sdin(saved_stdin);
	 if (*fd != 1)
		 close(*fd);
 }

 /**
  * @brief Builtin cd command implementation.
  *
  * Handles I/O redirection, argument checking, changes directory,
  * updates environment variables, and performs final cleanup.
  *
  * @param cmd Pointer to current command structure
  * @param env Pointer to shell environment structure
  * @param head Pointer to pipe list for memory management
  * @return 0 on success, -1 on failure
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


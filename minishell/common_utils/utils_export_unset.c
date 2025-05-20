/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:44:40 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:59:08 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Sorts an array of strings (environment variables) in lexicographical (alphabetical) order.
 *
 * Uses a simple bubble sort algorithm to sort the strings pointed by env.
 * The function modifies the passed array directly and returns it.
 *
 * @param env NULL-terminated array of strings to sort
 * @return char** sorted array (same instance passed as input)
 */
 char	**ft_env_sorted(char **env)
 {
	 int		i;
	 int		swapped;
	 char	*swap;

	 i = 0;
	 swap = NULL;
	 swapped = 1;
	 while (swapped)
	 {
		 swapped = 0;
		 i = 0;
		 while (env[i] && env[i + 1])
		 {
			 if (ft_strcmp(env[i], env[i + 1]) > 0)
			 {
				 swap = env[i];
				 env[i] = env[i + 1];
				 env[i + 1] = swap;
				 swapped = 1;
			 }
			 i++;
		 }
	 }
	 return (env);
 }

 /**
  * @brief Checks if a string is a valid identifier for an environment variable.
  *
  * Validity rules:
  * - Not empty and not NULL
  * - The first character must be a letter, '_' or one of the characters '\'' or '"' or space
  * - Other characters up to '=' must be alphanumeric, '_' or '\'' or '"' or space
  *
  * @param str string to validate
  * @return true if valid, false otherwise
  */
 bool	is_valid_identifier(char *str)
 {
	 int	i;
	 int	j;

	 i = 0;
	 j = 0;
	 if (!str || !str[0])
		 return (false);
	 while (str[i] && str[i] != '=')
		 i++;
	 if (!ft_isalpha(str[0]) && str[0] != '_'
		 && str[0] != '\'' && str[0] != '\"' && str[0] != ' ')
		 return (false);
	 while (j < i)
	 {
		 if (!ft_isalnum(str[j]) && str[j] != '_' && str[j] != '\''
			 && str[j] != '\"' && str[j] != ' ')
			 return (false);
		 j++;
	 }
	 return (true);
 }

 /**
  * @brief Searches for the index of an environment variable already present in env->env_copy.
  *
  * Searches for a variable with the same name (part before '=') as the string args.
  * Returns the index if found, -1 otherwise.
  *
  * @param env environment structure containing env_copy (array of strings)
  * @param args string containing the variable to search (e.g. "PATH=/bin")
  * @return int index if found, -1 if not found or error
  */
 int	ft_find_export_var(t_env *env, char *args)
 {
	 int	len;
	 int	i;

	 len = 0;
	 i = 0;
	 if (!args || !env->env_copy)
		 return (-1);
	 while (args[len] && args[len] != '=')
		 len++;
	 while (env->env_copy && env->env_copy[i])
	 {
		 if (!ft_strncmp(env->env_copy[i], args, len)
			 && env->env_copy[i][len] == '=')
			 return (i);
		 else if (!ft_strncmp(env->env_copy[i], args, len))
			 return (i);
		 i++;
	 }
	 return (-1);
 }

 /**
  * @brief Adds a new variable to the environment (env->env_copy).
  *
  * Allocates a new array with one more slot, copies existing variables, and adds the new variable
  * at the end, updating env->env_copy.
  *
  * If reallocation fails, prints an error and sets exit_status.
  *
  * @param env environment structure
  * @param args array of strings of variables to add (e.g. ["VAR=val", ...])
  * @param i index of the variable in args to add
  */
 void	ft_add_var(t_env *env, char **args, int *i)
 {
	 int		j;
	 char	**new_env_copy;

	 j = 0;
	 new_env_copy = NULL;
	 while (env->env_copy[j])
		 j++;
	 new_env_copy = ft_realloc_mtx(env->env_copy, sizeof(char *) * (j + 2), j);
	 if (!new_env_copy)
	 {
		 ft_printf(2, "Memory allocation failed!\n");
		 env->exit_status = 1;
		 return ;
	 }
	 env->env_copy = new_env_copy;
	 env->env_copy[j] = ft_strdup(args[(*i)]);
	 env->env_copy[j + 1] = NULL;
 }

 /**
  * @brief Checks if the argument passed to unset is valid.
  *
  * Validity rules:
  * - Not NULL
  * - First character must be a letter
  * - Must not contain '='
  * - Must not contain '/' or ':'
  * - All characters must be alphanumeric or '_'
  *
  * @param arg argument to check
  * @return 1 if valid, 0 otherwise
  */
 int	is_valid_unset_arg(char *arg)
 {
	 int	i;

	 i = 0;
	 if (!arg || (!ft_isalpha(arg[0])))
		 return (0);
	 while (arg[i])
	 {
		 if (arg[i] == '=')
			 return (0);
		 if (!ft_isalnum(arg[i]) && arg[i] != '_'
			 && (arg[i] == '/' || arg[i] == ':'))
			 return (0);
		 i++;
	 }
	 return (1);
 }

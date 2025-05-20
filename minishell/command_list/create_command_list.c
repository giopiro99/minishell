/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:16:40 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:52:11 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include <threads.h>

/**
 * @brief Expands the exit status variable `$?` in a string.
 *
 * Splits the string into three parts: the part before `$?`, the exit status converted to string,
 * and the part after `$?`. Then concatenates them and returns the result.
 *
 * @param value String containing `$?` to expand
 * @param env Shell environment (holds `exit_status`)
 * @return New string with `$?` expanded to exit status value
 */
 char	*ft_estatus_to_str(char *value, t_env *env)
 {
	 int		j;
	 char	*before_status;
	 char	*post_status;
	 char	*e_status;
	 char	*result;

	 before_status = NULL;
	 post_status = NULL;
	 j = 0;
	 while (value[j] && value[j] != '$')
		 j++;
	 before_status = ft_substr(value, 0, j);
	 j += 2; // skip past "$?"
	 e_status = ft_itoa(env->exit_status);
	 post_status = ft_substr(value, j, ft_strlen(value));
	 result = ft_strjoin_three(before_status, e_status, post_status);
	 free(e_status);
	 free(before_status);
	 free(post_status);
	 return (result);
 }

 /**
  * @brief Adds a new argument to the argument array.
  *
  * Handles expansion of environment variables or the exit status `$?` depending on token type.
  *
  * @param args Current array of arguments
  * @param value Value of the new argument to add
  * @param type Token type (e.g., ENV_VAR, EXIT_STATUS)
  * @param env Shell environment
  * @return New argument array with the added value, or NULL on failure
  */
 char	**add_args(char **args, char *value, t_type type, t_env *env)
 {
	 int		i;
	 char	**new_args;

	 i = ft_matrix_len(args);
	 new_args = ft_realloc_mtx(args, ft_matrix_len(args) + 2,
			 ft_matrix_len(args));
	 if (!new_args)
		 return (NULL);
	 if (type == ENV_VAR)
		 new_args[i++] = ft_expand_envvar(value, env);
	 else if (type == EXIT_STATUS)
		 new_args[i++] = ft_estatus_to_str(value, env);
	 else if (value)
		 new_args[i++] = ft_strdup(value);
	 else
		 new_args[i++] = ft_strdup("");
	 new_args[i] = NULL;
	 return (new_args);
 }

 /**
  * @brief Executes a redirection for the current command.
  *
  * Calls `handle_redirection` and sets the `open_error` flag if an error occurs.
  *
  * @param current Pointer to the current command
  * @param tokens Token containing the redirection
  * @param env Shell environment
  * @return Type of redirection handled, or -1 on error
  */
 static t_type	ft_exe_redir(t_cmd *current, t_token *tokens, t_env *env)
 {
	 t_type	redir_type;

	 redir_type = handle_redirection(current, tokens->type, tokens, env);
	 if (redir_type == (t_type) - 1)
	 {
		 if (current)
			 current->open_error = true;
		 return ((t_type) - 1);
	 }
	 return (redir_type);
 }

 /**
  * @brief Creates a linked list of commands from a list of tokens.
  *
  * Parses each token and constructs the `t_cmd` nodes,
  * handling commands, arguments, and redirections.
  *
  * @param tokens List of tokens to process
  * @param env Shell environment
  * @return Pointer to the head of the constructed command list
  */
 t_cmd	*create_list(t_token *tokens, t_env *env)
 {
	 t_cmd			*head;
	 t_cmd			*current;
	 t_redir_data	redir;

	 head = NULL;
	 current = NULL;
	 init_redir_data(&redir);
	 while (tokens)
	 {
		 if (tokens->type == COMMAND)
			 process_command(tokens, &current, &head, &redir);
		 else if (is_argument(tokens) && current)
			 current->args = add_args(current->args,
					 tokens->value, tokens->type, env);
		 else if (is_redir(tokens) || tokens->type == HEREDOC)
		 {
			 redir.type = ft_exe_redir(current, tokens, env);
			 redir.token = tokens;
		 }
		 tokens = tokens->next;
	 }
	 return (head);
 }

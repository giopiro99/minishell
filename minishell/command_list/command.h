/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:57:20 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:51:31 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "../common_utils/common_utils.h"
# include "../tokens/tokens.h"
# include "../redirections/redirections.h"

/**
 * @struct s_redir_data
 * @brief Structure to temporarily store redirection data.
 *
 * Contains the token associated with the redirection and the redirection type.
 */
 typedef struct s_redir_data
 {
	 t_token	*token;  /**< Token associated with the redirection */
	 t_type	type;    /**< Type of the redirection */
 }	t_redir_data;

 /**
  * @brief Adds a new command node to the list and updates the current node.
  *
  * @param current Pointer to the current command node
  * @param tokens Token containing the command
  * @param head Pointer to the head of the command list
  */
 void	ft_add_node(t_cmd **current, t_token *tokens, t_cmd **head);

 /**
  * @brief Initializes the redirection data.
  *
  * Sets the token to NULL and the type to -1.
  *
  * @param redir Pointer to the t_redir_data structure to initialize
  */
 void	init_redir_data(t_redir_data *redir);

 /**
  * @brief Processes a command token and handles any associated redirections.
  *
  * @param token Token containing the command
  * @param current Pointer to the current command node
  * @param head Pointer to the head of the command list
  * @param redir Temporary data related to the redirection to apply
  */
 void	process_command(t_token *token, t_cmd **current,
			 t_cmd **head, t_redir_data *redir);

 /**
  * @brief Adds an argument to the command’s argument list.
  *
  * Also handles expansion of environment variables or exit status.
  *
  * @param args Array of strings containing current arguments
  * @param value Argument value to add
  * @param type Token type (e.g., ENV_VAR, EXIT_STATUS)
  * @param env Shell environment
  * @return New argument array with the value added, or NULL on error
  */
 char	**add_args(char **args, char *value, t_type type, t_env *env);

 /**
  * @brief Adds a command node to the list.
  *
  * If the list is empty, sets the node as head; otherwise adds at the end.
  *
  * @param head Pointer to the head of the command list
  * @param new_token Command node to add
  */
 void	add_command(t_cmd **head, t_cmd *new_token);

 /**
  * @brief Creates a command list from tokens.
  *
  * Processes tokens, creates command nodes, adds arguments, and handles redirections.
  *
  * @param tokens List of tokens to process
  * @param env Shell environment
  * @return Pointer to the head of the created command list
  */
 t_cmd	*create_list(t_token *tokens, t_env *env);

 /**
  * @brief Creates a new initialized command node.
  *
  * @param cmd Command string to associate with the node
  * @param token Token related to the command
  * @return Pointer to the created node, or NULL on error
  */
 t_cmd	*create_node(char *cmd, t_token *token);


#endif


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:38:56 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:52:54 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

/**
 * @brief Creates a new command list node.
 *
 * Allocates and initializes a `t_cmd` structure with default values,
 * setting the command string and associated tokens.
 *
 * @param cmd Command string
 * @param token Token associated with the command
 * @return Pointer to the newly created `t_cmd` node, or NULL on allocation failure
 */
 t_cmd	*create_node(char *cmd, t_token *token)
 {
	 t_cmd	*node;

	 node = malloc(sizeof(t_cmd));
	 if (!node)
		 return (NULL);
	 node->args = malloc(sizeof(char *) + 1);
	 ft_bzero(node->args, sizeof(char *));
	 node->cmd = ft_strdup(cmd);
	 node->pipe_flag = false;
	 node->tokens = token;
	 node->next = NULL;
	 node->input = NULL;
	 node->output = NULL;
	 node->append = -1;
	 node->open_error = false;
	 return (node);
 }

 /**
  * @brief Adds a `t_cmd` node to the command list.
  *
  * If the list is empty, sets the new node as the head.
  * Otherwise, appends it at the end of the list.
  *
  * @param head Pointer to the head of the command list
  * @param new_token New `t_cmd` node to add
  */
 void	add_command(t_cmd **head, t_cmd *new_token)
 {
	 t_cmd	*tmp;

	 tmp = NULL;
	 if (!head || !new_token)
		 return ;
	 if (*head == NULL)
		 *head = new_token;
	 else
	 {
		 tmp = *head;
		 while (tmp->next)
			 tmp = tmp->next;
		 tmp->next = new_token;
	 }
 }

 /**
  * @brief Creates and adds a new command node to the list.
  *
  * Uses `create_node` to create the node and `add_command` to append it to the list.
  *
  * @param current Pointer to update with the new node
  * @param tokens Token containing the command
  * @param head Pointer to the head of the command list
  */
 void	ft_add_node(t_cmd **current, t_token *tokens, t_cmd **head)
 {
	 *current = create_node(tokens->value, tokens);
	 if (!*current)
		 return ;
	 add_command(head, *current);
 }

 /**
  * @brief Initializes a `t_redir_data` structure with default values.
  *
  * Sets `token` to NULL and `type` to -1 (invalid).
  *
  * @param redir Pointer to the `t_redir_data` structure to initialize
  */
 void	init_redir_data(t_redir_data *redir)
 {
	 redir->token = NULL;
	 redir->type = -1;
 }

 /**
  * @brief Processes a command token.
  *
  * Adds the command to the list and applies any previously stored redirection data
  * to the new node.
  *
  * @param token Token containing the command
  * @param current Pointer to the current command node
  * @param head Pointer to the head of the command list
  * @param redir Temporary redirection data to apply
  */
 void	process_command(t_token *token, t_cmd **current,
	 t_cmd **head, t_redir_data *redir)
 {
	 ft_add_node(current, token, head);
	 if (redir->token)
	 {
		 if (redir->type != (t_type) - 1)
			 ft_get_file_name(redir->token, token, *current, redir->type);
		 else
			 (*current)->open_error = true;
		 redir->token = NULL;
		 redir->type = -1;
	 }
 }

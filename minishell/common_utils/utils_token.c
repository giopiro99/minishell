/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:37:11 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:01:42 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"
#include <stdbool.h>

/**
 * @brief Creates a new token node.
 *
 * Dynamically allocates a token, duplicates the string value, and assigns the type.
 * The next pointer is initialized to NULL.
 *
 * @param value String to associate with the token (duplicated internally)
 * @param type Token type (t_type)
 * @return t_token* Pointer to the newly created token, or NULL if allocation fails
 */
 t_token	*new_token(const char *value, t_type type)
 {
	 t_token	*token;

	 token = malloc(sizeof(t_token));
	 if (!token)
		 return (NULL);
	 token->value = ft_strdup(value);
	 token->type = type;
	 token->next = NULL;
	 return (token);
 }

 /**
  * @brief Adds a token to the end of the linked list of tokens.
  *
  * If the list is empty, sets the new token as the head.
  *
  * @param head Double pointer to the head of the token list
  * @param new_token Pointer to the token to add
  */
 void	add_token(t_token **head, t_token *new_token)
 {
	 t_token	*tmp;

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
  * @brief Checks if the string contains the exit status variable "$?".
  *
  * Scans the string and returns true if the sequence "$?" is found.
  *
  * @param value String to check
  * @return true If it contains "$?"
  * @return false Otherwise
  */
 bool	exit_status_var(char *value)
 {
	 int	i;

	 i = 0;
	 while (value[i])
	 {
		 if (value[i] == '$' && value[i + 1] == '?')
			 return (true);
		 i++;
	 }
	 return (false);
 }

 /**
  * @brief Determines the token type based on the value, quote flag, and command flag.
  *
  * Uses conditions to recognize tokens such as quotes, environment variables, exit status,
  * arguments, pipes, and redirections.
  *
  * @param value Token string value
  * @param q_flag Quote flag (0 = none, 1 = single quote, 2 = double quote)
  * @param is_command Flag indicating if the token is a command (1) or not (0)
  * @return t_type Recognized token type
  */
 t_type	get_token_type(char *value, int q_flag, int is_command)
 {
	 if ((value[0] == '\'' || value[0] == '\"') && !ft_find_env_var(value))
		 return (QUOTE);
	 if (exit_status_var(value) && (q_flag == 0 || q_flag == 2))
		 return (EXIT_STATUS);
	 if ((ft_find_env_var(value) && (q_flag == 0 || q_flag == 2)))
		 return (ENV_VAR);
	 if (q_flag == 1 || q_flag == 2 || value[0] == '-')
		 return (ARGUMENT);
	 if (!ft_strcmp(value, "|"))
		 return (PIPE);
	 if (!ft_strcmp(value, ">>"))
		 return (REDIR_APPEND);
	 if (!ft_strcmp(value, "<<"))
		 return (HEREDOC);
	 if (!ft_strcmp(value, "<"))
		 return (REDIR_IN);
	 if (!ft_strcmp(value, ">"))
		 return (REDIR_OUT);
	 if (is_command == 1)
		 return (COMMAND);
	 return (ARGUMENT);
 }

 /**
  * @brief Removes a specific token node from the linked list.
  *
  * Frees the memory of the node and its value string.
  * Handles the case when the node to remove is the head of the list.
  *
  * @param head Double pointer to the head of the token list
  * @param target Pointer to the token to remove
  */
 void	remove_token_node(t_token **head, t_token *target)
 {
	 t_token	*curr;
	 t_token	*prev;

	 if (!head || !*head || !target)
		 return ;
	 curr = *head;
	 prev = NULL;
	 if (curr == target)
	 {
		 *head = curr->next;
		 free(curr->value);
		 free(curr);
		 return ;
	 }
	 while (curr && curr != target)
	 {
		 prev = curr;
		 curr = curr->next;
	 }
	 if (curr == target)
	 {
		 prev->next = curr->next;
		 free(curr->value);
		 free(curr);
	 }
 }

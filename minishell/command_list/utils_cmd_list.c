/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:38:56 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:39:58 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include <string.h>

/**
 * @brief Crea un nuovo nodo della lista di comandi.
 *
 * Alloca e inizializza una struttura `t_cmd` con i valori di default,
 * assegnando il comando e i token associati.
 *
 * @param cmd Comando da eseguire
 * @param token Token associato al comando
 * @return Puntatore al nodo `t_cmd` appena creato, o NULL in caso di errore
 */
t_cmd	*create_node(char *cmd, t_token *token)
{
	t_cmd	*node;

	node = malloc (sizeof(t_cmd));
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
 * @brief Aggiunge un nuovo nodo `t_cmd` alla lista dei comandi.
 *
 * Se la lista è vuota, imposta il nodo come testa.
 * Altrimenti, lo aggiunge in fondo alla lista.
 *
 * @param head Puntatore alla testa della lista dei comandi
 * @param new_token Nuovo nodo `t_cmd` da aggiungere
 */
void	add_command(t_cmd **head, t_cmd *new_token)
{
	t_cmd	*tmp;

	tmp = NULL;
	if (!head | !new_token)
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
 * @brief Crea e aggiunge un nuovo nodo comando alla lista.
 *
 * Usa `create_node` per creare il nodo e `add_command` per aggiungerlo
 * alla lista principale.
 *
 * @param current Puntatore al nodo corrente da aggiornare
 * @param tokens Token contenente il comando
 * @param head Puntatore alla testa della lista dei comandi
 */
void	ft_add_node(t_cmd **current, t_token *tokens, t_cmd **head)
{
	*current = create_node(tokens->value, tokens);
	if (!*current)
		return ;
	add_command(head, *current);
}

/**
 * @brief Inizializza la struttura `t_redir_data` con valori di default.
 *
 * Imposta `token` a NULL e `type` a -1.
 *
 * @param redir Puntatore alla struttura `t_redir_data` da inizializzare
 */
void	init_redir_data(t_redir_data *redir)
{
	redir->token = NULL;
	redir->type = -1;
}

/**
 * @brief Processa un token di tipo comando.
 *
 * Aggiunge il comando alla lista e gestisce eventuali redirezioni
 * precedentemente rilevate, associandole al nuovo nodo.
 *
 * @param token Token contenente il comando
 * @param current Puntatore al nodo comando corrente
 * @param head Puntatore alla testa della lista dei comandi
 * @param redir Dati temporanei della redirezione da applicare
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

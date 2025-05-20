/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:37:11 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:11:17 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"
#include <stdbool.h>

/**
 * @brief Crea un nuovo nodo token.
 *
 * Alloca dinamicamente un token, duplica la stringa value e assegna il tipo.
 * Il campo next è inizializzato a NULL.
 *
 * @param value Stringa da associare al token (duplicata internamente)
 * @param type Tipo del token (t_type)
 * @return t_token* Puntatore al nuovo token creato, o NULL in caso di errore di allocazione
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
 * @brief Aggiunge un token alla fine della lista concatenata di token.
 *
 * Se la lista è vuota, imposta il nuovo token come testa.
 *
 * @param head Doppio puntatore alla testa della lista token
 * @param new_token Puntatore al token da aggiungere
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;

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
 * @brief Controlla se la stringa contiene la variabile di exit status "$?".
 *
 * Scorre la stringa e restituisce true se trova la sequenza "$?".
 *
 * @param value Stringa da controllare
 * @return true Se contiene "$?"
 * @return false Altrimenti
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
 * @brief Determina il tipo di token dato un valore, il flag di quote e se è comando.
 *
 * Usa condizioni per riconoscere token come quote, variabili d'ambiente, exit status,
 * argomenti, pipe e redirezioni.
 *
 * @param value Stringa valore del token
 * @param q_flag Flag che indica il tipo di quote (0 = nessuna, 1 = singola, 2 = doppia)
 * @param is_command Flag che indica se il token è un comando (1) o no (0)
 * @return t_type Tipo del token riconosciuto
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
 * @brief Rimuove un nodo token specifico dalla lista concatenata.
 *
 * Libera la memoria del nodo e della sua stringa value.
 * Gestisce il caso in cui il nodo da rimuovere è la testa della lista.
 *
 * @param head Doppio puntatore alla testa della lista token
 * @param target Puntatore al token da rimuovere
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

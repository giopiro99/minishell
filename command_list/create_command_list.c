/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:16:40 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:38:56 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include <threads.h>

/**
 * @brief Espande il valore della variabile di stato di uscita `$?`.
 *
 * Divide la stringa in tre parti: prima di `$?`, lo stato di uscita convertito in stringa,
 * e la parte dopo. Poi le concatena e restituisce il risultato.
 *
 * @param value Stringa contenente `$?` da espandere
 * @param env Ambiente della shell (contiene `exit_status`)
 * @return Stringa risultante dall'espansione
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
	j += 2;
	e_status = ft_itoa(env->exit_status);
	post_status = ft_substr(value, j, ft_strlen(value));
	result = ft_strjoin_three(before_status, e_status, post_status);
	free(e_status);
	free(before_status);
	free(post_status);
	return (result);
}

/**
 * @brief Aggiunge un nuovo argomento all'array di argomenti.
 *
 * Gestisce anche l'espansione delle variabili d'ambiente o del valore di `$?`,
 * a seconda del tipo del token.
 *
 * @param args Array corrente di argomenti
 * @param value Valore del nuovo argomento da aggiungere
 * @param type Tipo del token (es. ENV_VAR, EXIT_STATUS)
 * @param env Ambiente della shell
 * @return Nuovo array con l'argomento aggiunto
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
 * @brief Esegue una redirezione per il comando corrente.
 *
 * Chiama `handle_redirection` e imposta il flag `open_error` in caso di errore.
 *
 * @param current Puntatore al comando corrente
 * @param tokens Token contenente la redirezione
 * @param env Ambiente della shell
 * @return Il tipo di redirezione gestita o -1 in caso di errore
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
 * @brief Crea una lista collegata di comandi a partire dalla lista dei token.
 *
 * Analizza ogni token e costruisce i nodi della lista `t_cmd`,
 * gestendo comandi, argomenti e redirezioni.
 *
 * @param tokens Lista dei token da elaborare
 * @param env Ambiente della shell
 * @return Puntatore alla testa della lista di comandi costruita
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

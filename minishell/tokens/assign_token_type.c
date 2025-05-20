/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:29:27 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:47:59 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Marca e gestisce i token di tipo SPACES in relazione ai token ARGUMENT, COMMAND, QUOTE, ENV_VAR e PIPE.
 *
 * Scorre la lista di token e aggiorna lo stato tra argomenti, chiamando
 * ft_assign_spaces per assegnare correttamente gli spazi.
 *
 * @param tokens Lista di token da processare.
 */
void	mark_sign_spaces(t_token *tokens)
{
	t_token	*prev;
	int		between_arg;

	prev = NULL;
	between_arg = 0;
	while (tokens)
	{
		if (tokens->type == ARGUMENT || tokens->type == COMMAND
			|| tokens->type == QUOTE || tokens->type == ENV_VAR
			|| tokens->type == PIPE)
		{
			if (between_arg)
				between_arg = 0;
			prev = tokens;
		}
		else if (tokens->type == SPACES)
			ft_assign_spaces(prev, tokens, &between_arg);
		tokens = tokens->next;
	}
}

/**
 * @brief Assegna correttamente i tipi ai token relativi a redirezioni e here-documents.
 *
 * Se è attivo il flag di redirezione, cerca il token ARGUMENT successivo e lo marca come FILE_.
 * Se il token è HEREDOC, cerca il token ARGUMENT successivo e lo marca come DELIMITER.
 * Gestisce anche l'attivazione/disattivazione dei flag redir_flag e is_first.
 *
 * @param redir_flag Puntatore al flag di redirezione.
 * @param tokens Il token corrente da processare.
 * @param is_first Puntatore al flag che indica se siamo al primo comando.
 * @return 0 se l'assegnazione è avvenuta, -1 altrimenti.
 */
static int	assign_redir(int *redir_flag, t_token *tokens, int *is_first)
{
	if (*redir_flag)
	{
		while (tokens->next && tokens->type != ARGUMENT)
			tokens = tokens->next;
		tokens->type = FILE_;
		*redir_flag = 0;
		*is_first = 1;
		return (0);
	}
	else if (tokens->type == HEREDOC)
	{
		while (tokens->next && tokens->type != ARGUMENT)
			tokens = tokens->next;
		if (tokens->type != HEREDOC && !is_space(tokens))
			tokens->type = DELIMITER;
		*is_first = 1;
		*redir_flag = 0;
		return (0);
	}
	else if (is_redir(tokens))
		return (*redir_flag = 1, *is_first = 0, 0);
	return (-1);
}

/**
 * @brief Assegna il tipo COMMAND o ARGUMENT a un token.
 *
 * Se è il primo token valido, assegna COMMAND, altrimenti ARGUMENT.
 * Aggiorna il flag is_first.
 *
 * @param tokens Il token da assegnare.
 * @param is_first Puntatore al flag che indica se siamo al primo comando.
 */
static void	assign_cmd(t_token *tokens, int *is_first)
{
	if (tokens->type == COMMAND || tokens->type == ARGUMENT)
	{
		if (*is_first)
			tokens->type = COMMAND;
		else
			tokens->type = ARGUMENT;
		*is_first = 0;
	}
}

/**
 * @brief Gestisce l'assegnazione del tipo per il token PIPE.
 *
 * Se il token è PIPE, controlla il token successivo:
 * - se non è PIPE, richiama ricorsivamente assign_token_types per assegnare i tipi.
 *
 * @param tokens Il token da controllare.
 * @return 0 se è stato un token PIPE e si è processato, -1 altrimenti.
 */
static int	assign_pipe(t_token *tokens)
{
	if (tokens->type == PIPE)
	{
		if (tokens->next)
			tokens = tokens->next;
		if (tokens->type != PIPE)
			assign_token_types(tokens);
		return (0);
	}
	return (-1);
}

/**
 * @brief Assegna i tipi corretti ai token della lista (COMMAND, ARGUMENT, FILE_, DELIMITER, PIPE).
 *
 * Scorre tutta la lista token e utilizza le funzioni ausiliarie per assegnare i tipi corretti
 * a seconda del contesto sintattico.
 *
 * @param tokens Lista di token da processare.
 */
void	assign_token_types(t_token *tokens)
{
	int	is_first;
	int	redir_flag;
	int	first_space;

	is_first = 1;
	redir_flag = 0;
	first_space = 1;
	while (tokens)
	{
		if (tokens->type == SPACES || tokens->type == QUOTE)
		{
			if (tokens->type == QUOTE)
				is_first = 0;
			if (tokens->next)
				tokens = tokens->next;
			else
				break ;
			continue ;
		}
		assign_redir(&redir_flag, tokens, &is_first);
		assign_cmd(tokens, &is_first);
		if (assign_pipe(tokens) == 0)
			break ;
		tokens = tokens->next;
	}
}

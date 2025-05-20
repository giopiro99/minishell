/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:10:58 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:19:30 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Espande le variabili d'ambiente in una linea e scrive il risultato su un file descriptor.
 *
 * @param line stringa di input da cui espandere le variabili (es. "echo $HOME")
 * @param env struttura con variabili d'ambiente (t_env)
 * @param fd file descriptor su cui scrivere la stringa espansa
 */
void	ft_for_env_var(char *line, t_env *env, int fd)
{
	char	*expanded;

	expanded = ft_expand_envvar(line, env);
	write(fd, expanded, ft_strlen(expanded));
	free (expanded);
}

/**
 * @brief Controlla se ci sono heredoc successivi nel token e rimuove il file temporaneo.
 *
 * Se il token successivo è di tipo HEREDOC, elimina il file temporaneo "temp_file.txt".
 * Serve per la gestione dell'eredoc in shell.
 *
 * @param tokens lista di token (linked list)
 */
void	ft_end_heredoc(t_token *tokens)
{
	if (tokens->next && ft_found_heredoc(tokens->next))
		unlink("temp_file.txt");
}

/**
 * @brief Assegna il nome del file di output e il flag di append in base al tipo di redirezione.
 *
 * Se il tipo è REDIR_OUT, salva il nome del file in cmd->output e append = 0.
 * Se il tipo è REDIR_APPEND, salva il nome del file e append = 1.
 *
 * @param cmd struttura comando da modificare
 * @param tokens token che contiene il nome del file (tokens->value)
 * @param type tipo di redirezione (REDIR_OUT o REDIR_APPEND)
 * @return true se il nome file è stato assegnato, false altrimenti
 */
bool	ft_get_outfile_name(t_cmd *cmd, t_token *tokens, t_type type)
{
	if (type == REDIR_OUT)
	{
		if (cmd->output)
			free(cmd->output);
		cmd->output = ft_strdup(tokens->value);
		cmd->append = 0;
		return (true);
	}
	else if (type == REDIR_APPEND)
	{
		if (cmd->output)
			free(cmd->output);
		cmd->output = ft_strdup(tokens->value);
		cmd->append = 1;
		return (true);
	}
	return (false);
}

/**
 * @brief Ottiene il nome del file da un token in base al tipo di redirezione e lo assegna al comando.
 *
 * Cerca il token che rappresenta un FILE_ o DELIMITER (per heredoc) e assegna il nome del file al comando:
 * - REDIR_OUT o REDIR_APPEND usa ft_get_outfile_name()
 * - REDIR_IN assegna cmd->input al nome del file
 * - HEREDOC assegna cmd->input a "temp_file.txt" (file temporaneo usato per heredoc)
 *
 * @param head testa della lista di token
 * @param tokens puntatore al token corrente (non usato come input ma reimpostato a head)
 * @param cmd struttura comando a cui assegnare il nome del file
 * @param type tipo di redirezione (REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC)
 * @return 0 sempre
 */
int	ft_get_file_name(t_token *head, t_token *tokens, t_cmd *cmd, t_type type)
{
	tokens = head;
	while (tokens && (tokens->type != FILE_ && tokens->type != DELIMITER))
		tokens = tokens->next;
	if (ft_get_outfile_name(cmd, tokens, type))
		return (0);
	else if (type == REDIR_IN)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup(tokens->value);
	}
	else if (type == HEREDOC)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup("temp_file.txt");
	}
	return (0);
}

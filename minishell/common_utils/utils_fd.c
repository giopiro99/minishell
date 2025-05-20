/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:15:12 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:15:05 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Gestisce l'apertura e la verifica dei file descriptor per input/output del comando.
 *
 * Se il comando ha un input rediretto (cmd->input), chiama handle_input_redirection per
 * gestire la redirezione dell'input e salvare lo stdin originale.
 * Successivamente determina il file descriptor di output con check_value_fd in base
 * a append_flag e output.
 * Controlla infine la validità del fd tramite ft_check_fd.
 *
 * @param cmd Puntatore al comando corrente
 * @param env Puntatore all'ambiente, per aggiornare exit_status in caso di errori
 * @param saved_stdin Puntatore per salvare lo stdin duplicato se necessario
 * @return int file descriptor valido o -1 in caso di errore
 */
int	ft_for_fd(t_cmd *cmd, t_env *env, int *saved_stdin)
{
	int	fd;

	fd = 1;
	if (cmd->input)
	{
		if (handle_input_redirection(cmd->input, saved_stdin) == -1)
			return (-1);
	}
	fd = check_value_fd(cmd->append, cmd->output, env);
	if (ft_check_fd(fd, env) == -1)
		return (-1);
	return (fd);
}

/**
 * @brief Controlla se un file descriptor è valido.
 *
 * Se fd == -1 indica errore nell'apertura del file, stampa perror, imposta exit_status,
 * e ritorna -1.
 * Altrimenti ritorna 0.
 *
 * @param fd file descriptor da controllare
 * @param env ambiente per aggiornare exit_status
 * @return int 0 se valido, -1 se errore
 */
int	ft_check_fd(int fd, t_env *env)
{
	if (fd == -1)
	{
		perror("open");
		env->exit_status = 1;
		return (-1);
	}
	return (0);
}

/**
 * @brief Restituisce il file descriptor corretto in base al flag di append e al file.
 *
 * - Se append_flag == 1: apre il file in modalità append (crea se non esiste)
 * - Se append_flag == 0: apre il file in modalità trunc (crea se non esiste)
 * - Se append_flag == -1: ritorna 1 (stdout)
 *
 * Controlla sempre la validità del fd tramite ft_check_fd.
 *
 * @param append_flag flag che indica il tipo di apertura (append/trunc/-1)
 * @param file nome del file su cui operare
 * @param env ambiente per aggiornare exit_status in caso di errore
 * @return int file descriptor aperto o -1 in caso di errore
 */
int	check_value_fd(int append_flag, char *file, t_env *env)
{
	int	fd;

	fd = 0;
	if (append_flag == 1)
	{
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0777);
		if (ft_check_fd(fd, env) == -1)
			return (-1);
	}
	else if (append_flag == 0)
	{
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (ft_check_fd(fd, env) == -1)
			return (-1);
	}
	else if (append_flag == -1)
		fd = 1;
	return (fd);
}

/**
 * @brief Verifica se nella lista di token è presente almeno un HEREDOC.
 *
 * Scorre la lista di token e ritorna true se trova un token di tipo HEREDOC.
 *
 * @param tokens puntatore alla lista di token
 * @return true se è presente un HEREDOC, false altrimenti
 */
bool	ft_found_heredoc(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == HEREDOC)
			return (true);
		tokens = tokens->next;
	}
	return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:57:54 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:53:53 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

/**
 * @brief Restituisce il valore del token usato come delimitatore del here-document.
 *
 * Cerca nella lista di token il primo token di tipo DELIMITER e ne restituisce il valore.
 *
 * @param tokens Lista dei token da analizzare
 * @return Il valore del delimitatore o NULL se non trovato
 */
char	*ft_found_delimiter(t_token *tokens)
{
	while (tokens && tokens->type != DELIMITER)
		tokens = tokens->next;
	if (tokens && tokens->value)
		return (tokens->value);
	return (NULL);
}

/**
 * @brief Esegue un singolo ciclo di lettura e scrittura del here-document.
 *
 * Legge una riga da stdin, verifica se è il delimitatore, e in caso contrario
 * la scrive su file. Gestisce anche le variabili d’ambiente e l’interruzione
 * tramite segnali.
 *
 * @param tokens Lista dei token, contenente il delimitatore e altre info
 * @param env Ambiente della shell
 * @param fd File descriptor del file temporaneo
 * @return true se l’heredoc deve essere interrotto, false altrimenti
 */
bool	ft_exe_heredoc(t_token *tokens, t_env *env, int *fd)
{
	char	*line;
	char	*line_n;
	char	*tmp;

	tmp = ft_strdup(ft_found_delimiter(tokens));
	line_n = ft_strjoin(tmp, "\n");
	free(tmp);
	line = get_next_line(0);
	if (!line)
	{
		ft_printf(2,
			"minishell: warning:"
			"here-document delimited by end-of-file wanted %s", line_n);
		return (free(line), free(line_n), close(*fd), true);
	}
	if (!ft_strcmp(line, line_n) || g_signal_flag == 130)
		return (free(line), free(line_n), close(*fd), true);
	else if (ft_found_heredoc(tokens))
		ft_for_env_var(line, env, *fd);
	else
		write(*fd, line, ft_strlen(line));
	free (line);
	free(line_n);
	return (false);
}

/**
 * @brief Gestisce l'intero processo di input per un here-document.
 *
 * Apre un file temporaneo e lo popola leggendo da stdin fino al
 * raggiungimento del delimitatore. Supporta anche l'interruzione
 * con segnali come SIGINT.
 *
 * @param tokens Lista dei token del comando corrente
 * @param env Puntatore alla struttura dell’ambiente della shell
 * @return 0 in caso di successo, -1 in caso di errore
 */
int	handle_heredoc(t_token *tokens, t_env *env)
{
	int		fd;
	int		saved_stdin;

	saved_stdin = 0;
	fd = open("temp_file.txt", O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (ft_check_fd(fd, env) == -1)
		return (-1);
	while (1)
	{
		if (ft_exe_heredoc(tokens, env, &fd))
			return (0);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:57:54 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:06:04 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

/**
 * @brief Returns the value of the token used as the here-document delimiter.
 *
 * Searches the token list for the first token of type DELIMITER and returns its value.
 *
 * @param tokens List of tokens to analyze
 * @return The delimiter value or NULL if not found
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
  * @brief Executes a single read-write cycle of the here-document.
  *
  * Reads a line from stdin, checks if it matches the delimiter, otherwise
  * writes it to the temporary file. Also handles environment variables
  * and interruption via signals.
  *
  * @param tokens List of tokens, containing the delimiter and other info
  * @param env Shell environment
  * @param fd File descriptor of the temporary file
  * @return true if the heredoc should be interrupted, false otherwise
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
			 "minishell: warning: "
			 "here-document delimited by end-of-file wanted %s", line_n);
		 return (free(line), free(line_n), close(*fd), true);
	 }
	 if (!ft_strcmp(line, line_n) || g_signal_flag == 130)
		 return (free(line), free(line_n), close(*fd), true);
	 else if (ft_found_heredoc(tokens))
		 ft_for_env_var(line, env, *fd);
	 else
		 write(*fd, line, ft_strlen(line));
	 free(line);
	 free(line_n);
	 return (false);
 }

 /**
  * @brief Handles the entire input process for a here-document.
  *
  * Opens a temporary file and fills it by reading from stdin until the
  * delimiter is reached. Supports interruption via signals such as SIGINT.
  *
  * @param tokens List of tokens of the current command
  * @param env Pointer to the shell environment structure
  * @return 0 on success, -1 on error
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

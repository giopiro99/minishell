/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:07:12 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:25:01 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_UTILS_H
# define COMMON_UTILS_H

# include "../libft/libft.h"
# include "signal_flag.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/**
 * @enum t_type
 * @brief Tipi di token/processi riconosciuti nella shell
 */
typedef enum s_type
{
	PIPE,          /**< Pipe "|" */
	REDIR_IN,      /**< Redirezione input "<" */
	REDIR_OUT,     /**< Redirezione output ">" */
	REDIR_APPEND,  /**< Redirezione output in append ">>" */
	ENV_VAR,       /**< Variabile d'ambiente */
	HEREDOC,       /**< Here-document */
	COMMAND,       /**< Comando */
	ARGUMENT,      /**< Argomento */
	FILE_,         /**< Nome file */
	DELIMITER,     /**< Delimitatore (es. per heredoc) */
	QUOTE,         /**< Carattere di quote */
	EXIT_STATUS,   /**< Variabile $? */
	SPACES,        /**< Spazi */
	GARBAGE        /**< Token non riconosciuto */
}	t_type;

/**
 * @struct s_exit
 * @brief Stato di uscita (exit status) del processo
 */
typedef struct s_exit
{
	int	exit_status; /**< Codice di uscita */
}	t_exit;

/**
 * @struct s_env
 * @brief Struttura per la gestione dell'ambiente e stato della shell
 */
typedef struct s_env
{
	char	**env_copy;    /**< Copia dell'ambiente come matrice di stringhe */
	int		exit_status;  /**< Exit status ultimo comando */
}	t_env;

/**
 * @struct s_token
 * @brief Nodo lista collegata per token della shell
 */
typedef struct s_token
{
	char			*value; /**< Valore testuale del token */
	t_type			type;   /**< Tipo del token */
	struct s_token	*next;  /**< Puntatore al token successivo */
}	t_token;

typedef struct s_pipe	t_pipe; /**< Forward declaration per pipe */

/**
 * @struct s_command
 * @brief Nodo lista collegata per comandi con relativi argomenti e redirezioni
 */
typedef struct s_command
{
	char				*cmd;        /**< Comando (es. "ls") */
	char				**args;     /**< Array di argomenti */
	char				*input;     /**< File di input */
	char				*output;    /**< File di output */
	int					append;     /**< Flag append: 0 truncate, 1 append, -1 nessuno */
	bool				pipe_flag;  /**< Flag se è parte di una pipe */
	bool				open_error; /**< Flag errore apertura file */
	t_token				*tokens;    /**< Lista token associati */
	struct s_command	*next;       /**< Puntatore al comando successivo */
}	t_cmd;

/**
 * @struct s_pipe
 * @brief Nodo lista collegata per pipe, con PID e file descriptor
 */
typedef struct s_pipe
{
	pid_t			pid;         /**< PID del processo */
	int				pipefd[2];   /**< File descriptor pipe [read, write] */
	t_cmd			*cmd;        /**< Comando associato */
	t_token			*tokens;     /**< Lista token associati */
	bool			one_cmd_flag;/**< Flag se pipe con un solo comando */
	struct s_pipe	*next;       /**< Puntatore alla pipe successiva */
}	t_pipe;

// Funzioni per file descriptor
int		check_value_fd(int append_flag, char *file, t_env *env);
int		ft_check_fd(int fd, t_env *env);
bool	ft_found_heredoc(t_token *tokens);
int		ft_for_fd(t_cmd *cmd, t_env *env, int *saved_stdin);
int		handle_input_redirection(char *input_file, int	*saved_stdin);
void	restore_sdin(int *saved_stdin);

// Funzioni di free
void	free_token(t_token *token);
void	free_cmd(t_cmd *cmd_list);
void	free_pipe(t_pipe *pipe);
void	free_all(t_cmd *cmd, t_env *env);
void	free_all_pipe(t_pipe *head_pipe, t_env *env);

// Funzioni per gestione quote
bool	ft_find_squotes(char *str);
bool	ft_find_dquotes(char *str);
bool	ft_find_env_var(char *str);

// Funzioni per variabili ambiente
char	*ft_get_env_value(t_env *env, const char *key);
char	*ft_expand_envvar(char *str, t_env *env);
char	*get_var_name(const char *env_var);
bool	check_only_env_var(t_token *tokens);

// Funzioni di stampa per debug
void	print_tokens(t_token *tokens);
void	print_command(t_cmd *command);

// Gestione segnali
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	get_exit_status(int status, t_env *env);

// Funzioni pipe
bool	has_pipe(t_token *token);
void	set_pipe_flag(t_cmd *head);

// Utils redirection
void	ft_for_redir(t_cmd *cmd, t_env *env, int fd, int *saved_stdin);
void	ft_for_env_var(char *line, t_env *env, int fd);
void	ft_end_heredoc(t_token *tokens);
int		ft_get_file_name(t_token *head, t_token *tokens,
			t_cmd *cmd, t_type type);
bool	ft_get_outfile_name(t_cmd *cmd, t_token *tokens, t_type type);

// Utils export
char	**ft_env_sorted(char **env);
bool	is_valid_identifier(char *str);
int		ft_find_export_var(t_env *env, char *args);
void	ft_add_var(t_env *env, char **args, int *i);
int		is_valid_unset_arg(char *arg);

// Utils token
t_type	get_token_type(char *value, int q_flag, int is_command);
bool	exit_status_var(char *value);
void	add_token(t_token **head, t_token *new_token);
t_token	*new_token(const char *value, t_type type);
bool	is_redir(t_token *tokens);
bool	is_argument(t_token *tokens);
bool	is_space(t_token *tokens);
bool	is_n_sequence(char *str);
void	ft_assign_spaces(t_token *prev, t_token *tokens, int *between_arg);
void	remove_token_node(t_token **head, t_token *target);

// Gestione errori
void	error_ex(const char *error);

#endif


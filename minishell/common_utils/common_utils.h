/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:07:12 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:54:09 by gpirozzi         ###   ########.fr       */
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
 * @brief Types of tokens/processes recognized by the shell
 */
typedef enum s_type
{
	PIPE,          /**< Pipe "|" */
	REDIR_IN,      /**< Input redirection "<" */
	REDIR_OUT,     /**< Output redirection ">" */
	REDIR_APPEND,  /**< Output redirection in append mode ">>" */
	ENV_VAR,       /**< Environment variable */
	HEREDOC,       /**< Here-document */
	COMMAND,       /**< Command */
	ARGUMENT,      /**< Argument */
	FILE_,         /**< File name */
	DELIMITER,     /**< Delimiter (e.g. for heredoc) */
	QUOTE,         /**< Quote character */
	EXIT_STATUS,   /**< Variable $? */
	SPACES,        /**< Spaces */
	GARBAGE        /**< Unrecognized token */
}	t_type;

/**
 * @struct s_exit
 * @brief Exit status of a process
 */
typedef struct s_exit
{
	int	exit_status; /**< Exit code */
}	t_exit;

/**
 * @struct s_env
 * @brief Structure to manage shell environment and state
 */
typedef struct s_env
{
	char	**env_copy;    /**< Copy of environment as string matrix */
	int		exit_status;  /**< Last command exit status */
}	t_env;

/**
 * @struct s_token
 * @brief Linked list node for shell tokens
 */
typedef struct s_token
{
	char			*value; /**< Text value of the token */
	t_type			type;   /**< Token type */
	struct s_token	*next;  /**< Pointer to next token */
}	t_token;

typedef struct s_pipe	t_pipe; /**< Forward declaration for pipe */

/**
 * @struct s_command
 * @brief Linked list node for commands with arguments and redirections
 */
typedef struct s_command
{
	char				*cmd;        /**< Command (e.g., "ls") */
	char				**args;      /**< Array of arguments */
	char				*input;      /**< Input file */
	char				*output;     /**< Output file */
	int					append;      /**< Append flag: 0 truncate, 1 append, -1 none */
	bool				pipe_flag;   /**< Flag if part of a pipe */
	bool				open_error;  /**< Flag for file opening error */
	t_token				*tokens;     /**< Associated tokens list */
	struct s_command		*next;       /**< Pointer to next command */
}	t_cmd;

/**
 * @struct s_pipe
 * @brief Linked list node for pipes, with PID and file descriptors
 */
typedef struct s_pipe
{
	pid_t			pid;          /**< Process ID */
	int				pipefd[2];    /**< Pipe file descriptors [read, write] */
	t_cmd			*cmd;         /**< Associated command */
	t_token			*tokens;      /**< Associated tokens */
	bool			one_cmd_flag; /**< Flag if pipe has only one command */
	struct s_pipe	*next;        /**< Pointer to next pipe */
}	t_pipe;

// File descriptor related functions
int		check_value_fd(int append_flag, char *file, t_env *env);
int		ft_check_fd(int fd, t_env *env);
bool	ft_found_heredoc(t_token *tokens);
int		ft_for_fd(t_cmd *cmd, t_env *env, int *saved_stdin);
int		handle_input_redirection(char *input_file, int *saved_stdin);
void	restore_sdin(int *saved_stdin);

// Memory free functions
void	free_token(t_token *token);
void	free_cmd(t_cmd *cmd_list);
void	free_pipe(t_pipe *pipe);
void	free_all(t_cmd *cmd, t_env *env);
void	free_all_pipe(t_pipe *head_pipe, t_env *env);

// Quote handling functions
bool	ft_find_squotes(char *str);
bool	ft_find_dquotes(char *str);
bool	ft_find_env_var(char *str);

// Environment variable functions
char	*ft_get_env_value(t_env *env, const char *key);
char	*ft_expand_envvar(char *str, t_env *env);
char	*get_var_name(const char *env_var);
bool	check_only_env_var(t_token *tokens);

// Debug print functions
void	print_tokens(t_token *tokens);
void	print_command(t_cmd *command);

// Signal handling
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	get_exit_status(int status, t_env *env);

// Pipe functions
bool	has_pipe(t_token *token);
void	set_pipe_flag(t_cmd *head);

// Redirection utilities
void	ft_for_redir(t_cmd *cmd, t_env *env, int fd, int *saved_stdin);
void	ft_for_env_var(char *line, t_env *env, int fd);
void	ft_end_heredoc(t_token *tokens);
int		ft_get_file_name(t_token *head, t_token *tokens,
			t_cmd *cmd, t_type type);
bool	ft_get_outfile_name(t_cmd *cmd, t_token *tokens, t_type type);

// Export utilities
char	**ft_env_sorted(char **env);
bool	is_valid_identifier(char *str);
int		ft_find_export_var(t_env *env, char *args);
void	ft_add_var(t_env *env, char **args, int *i);
int		is_valid_unset_arg(char *arg);

// Token utilities
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

// Error handling
void	error_ex(const char *error);

#endif



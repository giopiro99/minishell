/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:37:37 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:03:32 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Manages the execution flow of commands with pipes.
 *
 * Sets the pipe flag in commands, creates the pipe structure,
 * executes the piped commands, and frees the associated memory.
 *
 * @param head_token List of analyzed tokens
 * @param cmd_head List of commands to execute
 * @param env Shell environment
 */
 static void	ft_pipe_flow(t_token *head_token, t_cmd *cmd_head, t_env *env)
 {
	 t_pipe	*pipe;

	 set_pipe_flag(cmd_head);
	 pipe = create_pipe_list(cmd_head, head_token);
	 execute_pipe(pipe, env);
	 free_pipe(pipe);
 }

 /**
  * @brief Handles printing of a single environment variable.
  *
  * Searches and prints the value of the first environment variable found
  * in the token list.
  *
  * @param tokens Pointer to the list of tokens
  * @param env Shell environment
  */
 static void	ft_env_var_flow(t_token **tokens, t_env *env)
 {
	 t_token	*tmp;
	 char	*var;

	 tmp = *tokens;
	 var = NULL;
	 while (tmp->type != ENV_VAR && tmp->type != ARGUMENT)
		 tmp = tmp->next;
	 var = ft_expand_envvar(tmp->value, env);
	 ft_printf(1, "%s\n", var);
	 free(var);
 }

 /**
  * @brief Handles EOF event (Ctrl+D) by properly closing the shell.
  *
  * Frees the environment memory, closes file descriptors,
  * and exits the program with `exit(0)`.
  *
  * @param tmp Pointer to the environment to free
  */
 static void	ft_handle_ctrld(t_env *tmp)
 {
	 rl_clear_history();
	 ft_free_matrix(tmp->env_copy);
	 free(tmp);
	 ft_printf(1, "exit\n");
	 close(0);
	 close(1);
	 close(2);
	 exit(0);
 }

 /**
  * @brief Handles parsing and execution of tokens.
  *
  * Performs syntax and space checks, handles the case of only environment
  * variables or creates the command list. Depending on the presence of pipes,
  * executes the appropriate flow.
  *
  * @param tokens List of tokens generated from input
  * @param head_env Shell environment
  * @param cmd Pointer to the command list to update
  */
 void	ft_token_flow(t_token *tokens, t_env *head_env, t_cmd **cmd)
 {
	 t_cmd	*cmd_head;

	 cmd_head = *cmd;
	 if (!check_sintax(tokens) && !check_only_spaces(tokens))
	 {
		 if (check_only_env_var(tokens))
			 ft_env_var_flow(&tokens, head_env);
		 else
		 {
			 cmd_head = create_list(tokens, head_env);
			 if (has_pipe(tokens))
				 ft_pipe_flow(tokens, cmd_head, head_env);
			 else if (cmd_head)
				 ft_exe_cmd(cmd_head, head_env, cmd_head->tokens);
			 free_cmd(cmd_head);
		 }
	 }
 }

 /**
  * @brief Handles a complete cycle of input → parsing → execution.
  *
  * Receives user input with `readline`, updates history, and checks for
  * interruption signals. If present, tokenizes the input and calls the command flow.
  * Sets exit status on error.
  *
  * @param input Pointer to the input string to update
  * @param env Shell environment (pointer to pointer)
  */
 void	ft_main_flow(char **input, t_env **env)
 {
	 t_token	*tokens;
	 t_cmd	*cmd_head;
	 t_env	*tmp;

	 tokens = NULL;
	 cmd_head = NULL;
	 tmp = *env;
	 if (g_signal_flag == 130)
	 {
		 tmp->exit_status = 130;
		 g_signal_flag = 0;
	 }
	 *input = readline("\e[41;30m""minishell> ""\e[0m");
	 if (!*input)
		 ft_handle_ctrld(tmp);
	 if (*input)
		 add_history(*input);
	 tokens = tokenize(*input);
	 if (tokens)
	 {
		 ft_token_flow(tokens, tmp, &cmd_head);
		 free_token(tokens);
	 }
	 else
		 tmp->exit_status = 2;
 }


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:37:37 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:37:03 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Gestisce il flusso di esecuzione dei comandi con pipe.
 *
 * Imposta il flag di pipe nei comandi, crea la struttura delle pipe,
 * esegue i comandi pipati e libera la memoria associata.
 *
 * @param head_token Lista dei token analizzati
 * @param cmd_head Lista dei comandi da eseguire
 * @param env Ambiente della shell
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
 * @brief Gestisce la stampa di una singola variabile d'ambiente.
 *
 * Cerca e stampa il valore della prima variabile d'ambiente trovata
 * nella lista di token.
 *
 * @param tokens Puntatore alla lista dei token
 * @param env Ambiente della shell
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
 * @brief Gestisce l'evento EOF (Ctrl+D) chiudendo correttamente la shell.
 *
 * Libera la memoria dell'ambiente, chiude i file descriptor
 * e termina il programma con `exit(0)`.
 *
 * @param tmp Puntatore all'ambiente da liberare
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
 * @brief Gestisce l'analisi e l'esecuzione dei token.
 *
 * Esegue controlli di sintassi e spazi, gestisce il caso di sola variabile
 * d'ambiente o crea la lista comandi. In base alla presenza di pipe,
 * esegue il flusso appropriato.
 *
 * @param tokens Lista di token generata dall'input
 * @param head_env Ambiente della shell
 * @param cmd Puntatore alla lista di comandi da aggiornare
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
 * @brief Gestisce un ciclo completo di input → parsing → esecuzione.
 *
 * Riceve l'input utente con `readline`, aggiorna la cronologia e controlla
 * il segnale d'interruzione. Se presenti, tokenizza l'input e chiama il flusso
 * dei comandi. In caso di errore imposta lo stato d'uscita.
 *
 * @param input Puntatore alla stringa di input da aggiornare
 * @param env Ambiente della shell (puntatore a puntatore)
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

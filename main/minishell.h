/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:38:54 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:38:29 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../builtin/builtin.h"
# include "../tokens/tokens.h"
# include "../redirections/redirections.h"
# include "../common_utils/common_utils.h"
# include "../command_list/command.h"
# include "../system_commands/system_commands.h"
# include "../pipes/pipes.h"

/**
 * @brief Esegue i comandi (built-in o di sistema) presenti nella lista.
 *
 * Scorre la lista di comandi, identifica se sono built-in
 * o esterni, e li esegue di conseguenza.
 *
 * @param cmd_list Lista dei comandi da eseguire
 * @param env Puntatore alla struttura dell'ambiente
 * @param token Token associati al comando
 */
void	ft_exe_cmd(t_cmd *cmd_list, t_env *env, t_token *token);

/**
 * @brief Gestisce il ciclo principale del programma minishell.
 *
 * Legge l'input dell'utente, gestisce i segnali,
 * tokenizza l'input e avvia il flusso di esecuzione.
 *
 * @param input Puntatore alla stringa dell'input utente
 * @param env Puntatore alla struttura dell'ambiente
 */
void	ft_main_flow(char **input, t_env **env);

/**
 * @brief Gestisce l'elaborazione dei token e l'esecuzione dei comandi.
 *
 * Verifica la validità della sintassi, controlla se il comando
 * è solo una variabile d'ambiente e poi esegue il comando.
 *
 * @param tokens Lista dei token generati dal parser
 * @param head_env Puntatore all'ambiente
 * @param cmd Puntatore alla lista dei comandi
 */
void	ft_token_flow(t_token *tokens, t_env *head_env, t_cmd **cmd);

#endif


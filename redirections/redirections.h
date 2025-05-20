/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:47:39 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 11:35:14 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "../common_utils/common_utils.h"

/**
 * @brief Gestisce una redirezione all'interno di un comando.
 *
 * A seconda del tipo di redirezione (>, >>, <, <<),
 * esegue l'operazione appropriata modificando eventualmente
 * i file descriptor del comando.
 *
 * @param cmd Struttura del comando corrente
 * @param type Tipo di redirezione da gestire
 * @param tokens Lista dei token associati alla redirezione
 * @param env Struttura dell'ambiente della shell
 * @return Il tipo di redirezione gestito o ERROR in caso di fallimento
 */
t_type	handle_redirection(t_cmd *cmd, t_type type,
			t_token *tokens, t_env *env);

/**
 * @brief Gestisce una redirezione anche in assenza di comando associato.
 *
 * È utile per casi come un here-document standalone, ad esempio `<< DELIM`.
 *
 * @param type Tipo di redirezione
 * @param tokens Lista dei token associati alla redirezione
 * @param env Ambiente della shell
 * @return 0 in caso di successo, -1 in caso di errore
 */
int		handle_redirection_without_command(t_type type,
			t_token *tokens, t_env *env);

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
int		handle_heredoc(t_token *tokens, t_env *env);

/**
 * @brief Restituisce il valore del token usato come delimitatore del here-document.
 *
 * Cerca nella lista di token il primo token di tipo DELIMITER e ne restituisce il valore.
 *
 * @param tokens Lista dei token da analizzare
 * @return Il valore del delimitatore o NULL se non trovato
 */
char	*ft_found_delimiter(t_token *tokens);

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
bool	ft_exe_heredoc(t_token *tokens, t_env *env, int *fd);

#endif

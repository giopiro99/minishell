/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:57:20 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:24:41 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "../common_utils/common_utils.h"
# include "../tokens/tokens.h"
# include "../redirections/redirections.h"

/**
 * @struct s_redir_data
 * @brief Struttura per memorizzare temporaneamente i dati di una redirezione.
 *
 * Contiene il token associato alla redirezione e il tipo di redirezione.
 */
typedef struct s_redir_data
{
	t_token	*token;  /**< Token associato alla redirezione */
	t_type	type;    /**< Tipo di redirezione */
}	t_redir_data;

/**
 * @brief Aggiunge un nuovo nodo comando alla lista e aggiorna il nodo corrente.
 *
 * @param current Puntatore al nodo comando corrente
 * @param tokens Token contenente il comando
 * @param head Puntatore alla testa della lista dei comandi
 */
void	ft_add_node(t_cmd **current, t_token *tokens, t_cmd **head);

/**
 * @brief Inizializza i dati della redirezione.
 *
 * Imposta il token a NULL e il tipo a -1.
 *
 * @param redir Puntatore alla struttura t_redir_data da inizializzare
 */
void	init_redir_data(t_redir_data *redir);

/**
 * @brief Processa un token comando e gestisce eventuali redirezioni associate.
 *
 * @param token Token contenente il comando
 * @param current Puntatore al nodo comando corrente
 * @param head Puntatore alla testa della lista dei comandi
 * @param redir Dati temporanei relativi alla redirezione da applicare
 */
void	process_command(t_token *token, t_cmd **current,
			t_cmd **head, t_redir_data *redir);

/**
 * @brief Aggiunge un argomento alla lista degli argomenti di un comando.
 *
 * Gestisce anche l'espansione di variabili d'ambiente o dell'exit status.
 *
 * @param args Array di stringhe contenente gli argomenti attuali
 * @param value Valore dell'argomento da aggiungere
 * @param type Tipo del token (es. ENV_VAR, EXIT_STATUS)
 * @param env Ambiente della shell
 * @return Nuovo array di argomenti con il valore aggiunto, NULL in caso di errore
 */
char	**add_args(char **args, char *value, t_type type, t_env *env);

/**
 * @brief Aggiunge un nodo comando alla lista.
 *
 * Se la lista è vuota, imposta il nodo come testa, altrimenti aggiunge in coda.
 *
 * @param head Puntatore alla testa della lista dei comandi
 * @param new_token Nodo comando da aggiungere
 */
void	add_command(t_cmd **head, t_cmd *new_token);

/**
 * @brief Crea una lista di comandi a partire dai token.
 *
 * Processa i token, crea nodi comandi, aggiunge argomenti e gestisce redirezioni.
 *
 * @param tokens Lista di token da processare
 * @param env Ambiente della shell
 * @return Puntatore alla testa della lista di comandi creata
 */
t_cmd	*create_list(t_token *tokens, t_env *env);

/**
 * @brief Crea un nuovo nodo comando inizializzato.
 *
 * @param cmd Comando da associare al nodo
 * @param token Token relativo al comando
 * @return Puntatore al nodo creato, o NULL in caso di errore
 */
t_cmd	*create_node(char *cmd, t_token *token);

#endif


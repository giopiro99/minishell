/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:22:18 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:01:19 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../libft/libft.h"
# include "../common_utils/common_utils.h"
# include "../redirections/redirections.h"

/**
 * @brief Funzione builtin per il comando cd (change directory).
 *
 * @param cmd  Puntatore alla struttura comando corrente
 * @param env  Puntatore alla struttura ambiente shell
 * @param head Puntatore alla lista pipe (per gestione memoria)
 * @return int 0 se successo, -1 in caso di errore
 */
int		ft_cd(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Cerca una variabile da esportare nell'ambiente.
  *
  * @param env  Puntatore alla struttura ambiente shell
  * @param args Stringa variabile ambiente da cercare
  * @return int  indice della variabile trovata oppure -1 se non trovata
  */
int		ft_find_export_var(t_env *env, char *args);

 /**
  * @brief Funzione builtin per il comando echo.
  *
  * Stampa gli argomenti passati su stdout o file descriptor specificato.
  *
  * @param cmd Puntatore alla struttura comando
  * @param token Puntatore ai token della linea di comando
  * @param env Puntatore alla struttura ambiente shell
  * @param head Puntatore alla lista pipe
  */
void	ft_echo(t_cmd *cmd, t_token *token, t_env *env, t_pipe *head);

 /**
  * @brief Funzione builtin per il comando pwd (print working directory).
  *
  * Stampa la directory di lavoro corrente.
  *
  * @param cmd Puntatore alla struttura comando
  * @param env Puntatore alla struttura ambiente shell
  * @param head Puntatore alla lista pipe
  */
void	ft_pwd(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Funzione builtin per il comando unset.
  *
  * Rimuove variabili dall'ambiente.
  *
  * @param cmd Puntatore alla struttura comando
  * @param env Puntatore alla struttura ambiente shell
  * @param head Puntatore alla lista pipe
  */
void	ft_unset(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Funzione builtin per il comando exit.
  *
  * Termina la shell con il codice di uscita specificato.
  *
  * @param cmd Puntatore alla struttura comando
  * @param env Puntatore alla struttura ambiente shell
  * @param head Puntatore alla lista pipe
  */
void	ft_exit(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Funzione builtin per il comando env.
  *
  * Stampa o modifica le variabili d'ambiente.
  *
  * @param cmd Puntatore alla struttura comando
  * @param env Puntatore alla struttura ambiente shell
  * @param head Puntatore alla lista pipe
  */
void	ft_env(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Funzione builtin per il comando export.
  *
  * Aggiunge o modifica variabili nell'ambiente export.
  *
  * @param cmd Puntatore alla struttura comando
  * @param env Puntatore alla struttura ambiente shell
  * @param head Puntatore alla lista pipe
  */
void	ft_export(t_cmd *cmd, t_env *env, t_pipe *head);

 /**
  * @brief Libera tutte le risorse allocate associate alla lista comandi e ambiente.
  *
  * @param cmd_list Puntatore alla lista comandi
  * @param env Puntatore alla struttura ambiente shell
  */
void	free_all(t_cmd *cmd_list, t_env *env);

 /**
  * @brief Setta o aggiorna una variabile d'ambiente.
  *
  * @param key Nome della variabile
  * @param var Valore da assegnare
  * @param env Puntatore alla struttura ambiente shell
  */
void	ft_setenv(const char *key, const char *var, t_env *env);

 /**
  * @brief Controlla se una stringa è un identificatore valido per variabili d'ambiente.
  *
  * @param str Stringa da verificare
  * @return true se valido, false altrimenti
  */
bool	is_valid_identifier(char *str);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_commands.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:06:04 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:52:35 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEM_COMMANDS_H
# define SYSTEM_COMMANDS_H

# include "../libft/libft.h"
# include "../common_utils/common_utils.h"
# include "../redirections/redirections.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

/**
 * @brief Cerca il percorso eseguibile di un comando.
 *
 * @param env Ambiente della shell contenente le variabili d’ambiente
 * @param cmd Struttura del comando con nome e argomenti
 * @return char* Percorso completo del comando o NULL se non trovato
 */
 char	*ft_path_finder(t_env *env, t_cmd *cmd);

 /**
  * @brief Esegue un comando di sistema creando un processo figlio.
  *
  * @param env Ambiente della shell contenente le variabili d’ambiente
  * @param cmd Struttura del comando da eseguire
  * @return int 0 se successo, codice di errore altrimenti
  */
 int		ft_exe_syscmd(t_env *env, t_cmd *cmd);

 /**
  * @brief Prepara la matrice di argomenti per execve.
  *
  * @param cmd Struttura del comando con argomenti
  * @return char** Matrice di argomenti terminata da NULL o NULL in caso di errore
  */
 char	**ft_mtx_cmd(t_cmd *cmd);

 #endif

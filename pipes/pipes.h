/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:54:55 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:07:25 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "../common_utils/common_utils.h"
# include "../libft/libft.h"
# include "../builtin/builtin.h"
# include "../redirections/redirections.h"
# include "../common_utils/common_utils.h"
# include "../system_commands/system_commands.h"

/**
 * @brief Crea un singolo nodo pipe a partire da un comando.
 *
 * @param cmd Comando da eseguire
 * @param head_tokens Token associati al comando
 * @return Nodo pipe creato
 */
t_pipe	*create_pipe_node(t_cmd *cmd, t_token *head_tokens);

/**
 * @brief Crea una lista concatenata di pipe a partire dalla lista di comandi.
 *
 * @param cmd Lista dei comandi
 * @param head_tokens Lista dei token
 * @return Testa della lista pipe
 */
t_pipe	*create_pipe_list(t_cmd *cmd, t_token *head_tokens);

/**
 * @brief Esegue un comando esterno tramite execve, reindirizzando stdout se necessario.
 *
 * @param fd File descriptor per l'output
 * @param dir Percorso eseguibile
 * @param matrix_exe Matrice degli argomenti
 * @param env Ambiente shell
 */
void	ft_execution_execve(int *fd, char *dir, char **matrix_exe, t_env *env);

/**
 * @brief Esegue un comando specifico in una pipeline.
 *
 * @param current Nodo corrente della pipe
 * @param env Ambiente della shell
 * @param token Token del comando
 * @param head Testa della lista pipe
 */
void	ft_exe_pipe_cmd(t_pipe *current, t_env *env,
			t_token *token, t_pipe *head);

/**
 * @brief Esegue un comando di sistema all'interno di una pipeline.
 *
 * @param env Ambiente della shell
 * @param cmd Comando da eseguire
 * @return Exit status del comando
 */
int		ft_exe_syscmd_in_pipe(t_env *env, t_cmd *cmd);

/**
 * @brief Esegue l'intera pipeline di comandi.
 *
 * @param head_pipe Testa della lista pipe
 * @param env Ambiente della shell
 */
void	execute_pipe(t_pipe *head_pipe, t_env *env);

/**
 * @brief Aggiunge un nodo alla lista delle pipe.
 *
 * @param head Testa della lista pipe
 * @param new_token Nuovo nodo da aggiungere
 */
void	add_pipe(t_pipe **head, t_pipe *new_token);

#endif


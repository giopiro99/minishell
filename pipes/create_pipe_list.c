/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:40:29 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:36:51 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"
#include <stdio.h>

/**
 * @brief Crea una lista concatenata di strutture pipe a partire dai comandi.
 *
 * Ogni comando della lista viene avvolto in un nodo `t_pipe`,
 * che viene poi collegato agli altri per formare una pipeline.
 *
 * @param cmd Lista dei comandi da eseguire
 * @param token Lista dei token associati
 * @return t_pipe* Testa della lista di pipe creata
 */
t_pipe	*create_pipe_list(t_cmd *cmd, t_token *token)
{
	t_pipe	*current_pipe;
	t_pipe	*head_pipe;

	current_pipe = NULL;
	head_pipe = NULL;
	while (cmd)
	{
		current_pipe = create_pipe_node(cmd, token);
		add_pipe(&head_pipe, current_pipe);
		cmd = cmd->next;
	}
	return (head_pipe);
}

/**
 * @brief Crea un nodo della pipeline contenente un comando.
 *
 * Alloca memoria e inizializza tutti i campi necessari, incluso
 * l'array dei file descriptor per la pipe.
 *
 * @param cmd Puntatore al comando da eseguire
 * @param head_token Lista dei token associati
 * @return t_pipe* Nodo della pipeline inizializzato
 */
t_pipe	*create_pipe_node(t_cmd *cmd, t_token *head_token)
{
	t_pipe	*node;

	node = malloc (sizeof(t_pipe));
	if (!node)
		return (NULL);
	node->pid = -1;
	node->cmd = cmd;
	node->tokens = head_token;
	node->pipefd[0] = -1;
	node->pipefd[1] = -1;
	node->one_cmd_flag = false;
	node->next = NULL;
	return (node);
}

/**
 * @brief Esegue un comando esterno con `execve`, reindirizzando stdout se necessario.
 *
 * Se `fd` è diverso da STDOUT, esegue `dup2` per il redirezionamento.
 * Poi lancia il binario usando `execve` con l’ambiente personalizzato.
 * In caso di errore stampa il messaggio e imposta `exit_status` a 126.
 *
 * @param fd Puntatore al file descriptor di output
 * @param dir Percorso assoluto del binario da eseguire
 * @param matrix_exe Matrice di argomenti per execve
 * @param env Ambiente della shell
 */
void	ft_execution_execve(int *fd, char *dir, char **matrix_exe, t_env *env)
{
	if (*fd != 1)
	{
		if (dup2(*fd, STDOUT_FILENO) == -1)
			return (perror("dup2"));
		close (*fd);
	}
	if (dir && execve(dir, matrix_exe, env->env_copy) == -1)
	{
		ft_free_matrix(matrix_exe);
		free(dir);
		env->exit_status = 126;
		perror("execve");
	}
}

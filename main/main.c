/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:39:26 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 10:16:08 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Funzione principale della minishell.
 *
 * Inizializza l'ambiente, imposta i segnali e gestisce il ciclo principale
 * che riceve l'input da utente, lo processa e lo esegue.
 *
 * @param ac Numero di argomenti passati da linea di comando (ignorato)
 * @param av Lista di argomenti da linea di comando (ignorato)
 * @param envp Lista delle variabili di ambiente fornite dal sistema
 * @return int Exit status (non viene mai raggiunto in condizioni normali)
 */
#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	char	*input;

	input = NULL;
	env = malloc(sizeof(t_env));
	if (!env)
		return (-1);
	(void)ac;
	(void)av;
	env->env_copy = NULL;
	env->env_copy = ft_dup_matrix(envp, env->env_copy);
	env->exit_status = 0;
	while (1)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		ft_main_flow(&input, &env);
		unlink("temp_file.txt");
	}
	free(input);
	return (0);
}

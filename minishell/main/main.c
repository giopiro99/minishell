/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:39:26 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:03:49 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Main function of the minishell.
 *
 * Initializes the environment, sets up signal handlers, and manages the main loop
 * that receives user input, processes it, and executes it.
 *
 * @param ac Number of command-line arguments (ignored)
 * @param av List of command-line arguments (ignored)
 * @param envp List of environment variables provided by the system
 * @return int Exit status (never normally reached)
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

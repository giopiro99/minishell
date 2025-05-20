/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:19:24 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:57:33 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

int	g_signal_flag = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl-C).
 *
 * When SIGINT is received, this function:
 * - Prints a newline to move to the next line
 * - Tells readline to move to a new line with rl_on_new_line()
 * - Replaces the current input line with an empty string (clears current prompt)
 * - Redisplays the prompt line with rl_redisplay()
 * - Sets the global variable g_signal_flag to 130 (standard exit code for SIGINT)
 *
 * @param sig the signal number received
 */
void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_flag = 130;
	}
}

/**
 * @brief Updates the child process exit status in the env struct.
 *
 * Takes the status returned by waitpid and updates env->exit_status based on:
 * - Process exited normally with exit code -> exit_status = exit code
 * - Process terminated by signal -> exit_status = 128 + signal number
 *   Also prints a message on the terminal if signal is SIGQUIT or SIGINT.
 *
 * Afterwards, sets signal handlers:
 * - SIGQUIT is ignored (SIG_IGN)
 * - SIGINT is handled by handle_sigint
 *
 * @param status status value returned by waitpid
 * @param env environment struct to store exit_status
 */
void	get_exit_status(int status, t_env *env)
{
	int	sig;

	sig = -1;
	if (WIFEXITED(status))
		env->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_printf(2, "Quit (core dumped)\n");
		else if (sig == SIGINT)
			ft_printf(2, "\n");
		env->exit_status = 128 + sig;
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
}


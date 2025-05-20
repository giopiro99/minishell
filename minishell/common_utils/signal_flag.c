/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:19:24 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:18:33 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

int	g_signal_flag = 0;

/**
 * @brief Handler per il segnale SIGINT (Ctrl-C).
 *
 * Quando si riceve SIGINT, questa funzione:
 * - Stampa una nuova linea per andare a capo
 * - Dice a readline di andare a una nuova linea con rl_on_new_line()
 * - Sostituisce la linea corrente vuota (cancella il prompt corrente)
 * - Ridisegna la linea prompt con rl_redisplay()
 * - Imposta la variabile globale g_signal_flag a 130 (codice di uscita standard per SIGINT)
 *
 * @param sig il numero del segnale ricevuto
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
 * @brief Aggiorna lo stato di uscita del processo figlio nel struct env.
 *
 * Prende lo status restituito da waitpid e aggiorna env->exit_status in base a:
 * - Processo terminato normalmente con exit code -> exit_status = codice di uscita
 * - Processo terminato da segnale -> exit_status = 128 + numero segnale
 *   Inoltre se il segnale è SIGQUIT o SIGINT, stampa un messaggio sul terminale.
 *
 * Dopo, imposta i signal handler:
 * - SIGQUIT viene ignorato (SIG_IGN)
 * - SIGINT viene gestito da handle_sigint
 *
 * @param status valore di stato restituito da waitpid
 * @param env struttura dell'ambiente per memorizzare exit_status
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

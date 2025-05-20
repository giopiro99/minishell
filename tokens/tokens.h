/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:33:43 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 12:05:27 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "../common_utils/common_utils.h"

/*
* Tokenizza la stringa di input, trasformandola in una lista collegata di token.
* Restituisce il puntatore alla testa della lista di token.
*/
t_token	*tokenize(char *input);

/*
* Determina il tipo di token in base al valore, alla presenza di virgolette
* e se il token è parte di un comando.
*/
t_type	get_token_type(char *value, int q_flag, int is_command);

/*
* Aggiunge un nuovo token alla lista collegata di token.
*/
void	add_token(t_token **head, t_token *new_token);

/*
* Assegna i tipi appropriati ai token (es. COMMAND, ARGUMENT, FILE_, ecc.)
* sulla base del contesto e della sequenza.
*/
void	assign_token_types(t_token *tokens);

/*
* Marca correttamente i token di tipo SPACES in relazione ai token adiacenti.
*/
void	mark_sign_spaces(t_token *tokens);

/*
* Unisce token ARGUMENT consecutivi nella lista di token per il comando 'export'.
*/
void	ft_merge_tokens_export(t_token **tokens);

/*
* Unisce opzioni '-n' multiple nei token ARGUMENT per il comando 'echo'.
*/
void	ft_merge_tokens_echo(t_token **tokens);

/*
 * Controlla la sintassi della lista di token e segnala eventuali errori.
 * Restituisce true se è presente un errore sintattico, false altrimenti.
 */
bool	check_sintax(t_token *tokens);

/*
* Verifica se la lista di token è composta solo da spazi o token inutili (GARBAGE).
* Restituisce true se è così, false altrimenti.
*/
bool	check_only_spaces(t_token *tokens);

/*
* Gestisce l'estrazione di stringhe racchiuse tra virgolette, copiandole in buffer.
* Aggiorna gli indici di input e buffer, e restituisce un flag di tipo.
 */
int		ft_for_quotes(char *input, int *i, int *j, char *buffer);

/* Funzioni di gestione specifica dei token in fase di parsing */

/*
 * Gestisce il parsing di stringhe racchiuse tra virgolette.
 * Restituisce true se il parsing ha successo, false in caso di errore.
 */
bool	handle_quotes(char *input, int *i, char *buffer, t_token **tokens);

/*
 * Gestisce i caratteri speciali come pipe e redirezioni durante la tokenizzazione.
 * Restituisce true se il parsing ha successo, false in caso di errore.
 */
bool	handle_special_chars(char *input, int *i, char *buffer, t_token **tokens);

/*
 * Gestisce la tokenizzazione delle parole normali (non speciali, non virgolette).
 * Restituisce true se il parsing ha successo, false in caso di errore.
 */
bool	handle_word(char *input, int *i, char *buffer, t_token **tokens);

/* Funzioni di controllo sintattico specifiche per tipi di token */

/* Controlla la correttezza sintattica delle redirezioni */
bool	check_redir(t_token *tmp);

/* Controlla la correttezza sintattica dell'heredoc */
bool	check_heredoc(t_token *tmp);

/* Controlla la correttezza sintattica della pipe */
bool	check_pipe(t_token *tmp);

/* Controlla la correttezza sintattica di un comando */
bool	check_command(t_token *tmp);

/* Verifica che i token siano solo spazi o token inutili */
bool	check_only_spaces(t_token *tokens);

#endif


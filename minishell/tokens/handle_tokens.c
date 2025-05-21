/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:49:02 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:36:33 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

/**
 * @brief Sets the quote type and corresponding flag.
 *
 * Determines if the quote is single or double and sets the flag
 * for further processing.
 *
 * @param quote Pointer to the character that will hold the quote type
 * @param input Input string to analyze
 * @param i Current index in the string
 * @param flag Pointer to the flag to set (1 for single quote, 2 for double quote)
 */
static void	ft_set_flag(char *quote, char *input, int *i, int *flag)
{
	*quote = input[(*i)++];
	if (*quote == '\'')
		*flag = 1;
	else
		*flag = 2;
}

 /**
  * @brief Processes a quoted string and copies it into the buffer.
  *
  * Handles both single and double quotes.
  *
  * @param input Input string to analyze
  * @param i Pointer to the current index in the input
  * @param j Pointer to the buffer index
  * @param buffer Buffer to copy the quoted content into
  * @return 1 for single quotes, 2 for double quotes, -1 in case of error
  */
int	ft_for_quotes(char *input, int *i, int *j, char *buffer)
{
	char	quote;
	int		flag;

	quote = 0;
	flag = 0;
	if (input[(*i)] == '\'' || input[(*i)] == '"')
	{
		ft_set_flag(&quote, input, i, &flag);
		while (input[(*i)] && input[(*i)] != quote)
		{
			if (input[(*i)] == quote)
				(*i)++;
			else
				buffer[(*j)++] = input[(*i)++];
		}
		if (input[(*i)] == '\0')
			 return (-1);
		(*i)++;
		if (j == 0)
			buffer[(*j)++] = '\0';
	}
	else
		buffer[(*j)++] = input[(*i)++];
	return (flag);
}

 /**
  * @brief Handles tokens enclosed in quotes and adds them to the list.
  *
  * Calls `ft_for_quotes`, manages possible errors from unclosed quotes,
  * and adds the new token to the list.
  *
  * @param input Input string to analyze
  * @param i Current index in the string
  * @param buffer Temporary buffer to build the token
  * @param tokens List of tokens to add the result to
  * @return true on success, false on error (unclosed quote)
  */
bool	handle_quotes(char *input, int *i, char *buffer, t_token **tokens)
{
	int		flag;
	int		j;

	j = 0;
	flag = ft_for_quotes(input, i, &j, buffer);
	if (flag == -1)
	{
		error_ex("minishell: QUOTE ERROR\n");
		free_token(*tokens);
		free(buffer);
		return (false);
	}
	buffer[j] = '\0';
	if (j > 0)
		add_token(tokens, new_token(buffer, get_token_type(buffer, flag, 0)));
	return (true);
}

 /**
  * @brief Handles shell special characters (|, <, >, <<, >>).
  *
  * Constructs the corresponding token and adds it to the token list.
  *
  * @param input Input string to analyze
  * @param i Current index in the string
  * @param buffer Temporary buffer to build the token
  * @param tokens List of tokens to add the result to
  * @return true always, no error handling performed
  */
bool	handle_special_chars(char *input, int *i,
					 char *buffer, t_token **tokens)
{
	int	j;

	j = 0;
	if (input[(*i)] == '|' || input[(*i)] == '<' || input[(*i)] == '>')
	{
		j = 0;
		if ((input[(*i)] == '<' && input[(*i) + 1] == '<')
			|| (input[(*i)] == '>' && input[(*i) + 1] == '>'))
		{
			buffer[j++] = input[(*i)++];
			buffer[j++] = input[(*i)++];
		}
		else
			buffer[j++] = input[(*i)++];
		buffer[j] = '\0';
		add_token(&(*tokens), new_token(buffer,
				get_token_type(buffer, 0, 0)));
	}
	return (true);
}

 /**
  * @brief Handles creation of a normal token (words, commands, arguments).
  *
  * Advances until encountering a space or special character.
  *
  * @param input Input string to analyze
  * @param i Current index in the string
  * @param buffer Temporary buffer to build the token
  * @param tokens List of tokens to add the result to
  * @return true always, no error handling performed
  */
bool	handle_word(char *input, int *i, char *buffer, t_token **tokens)
{
	int	j;
	int	flag;

	j = 0;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			flag = ft_for_quotes(input, i, &j, buffer);
			if (flag == -1)
				return (error_ex("minishell: QUOTE ERROR\n"),
				free_token(*tokens), free(buffer), false);
		}
		else
			buffer[j++] = input[(*i)++];
	}
	buffer[j] = '\0';
	if (j > 0)
		add_token(tokens, new_token(buffer, get_token_type(buffer, 0, 0)));
	return (true);
}


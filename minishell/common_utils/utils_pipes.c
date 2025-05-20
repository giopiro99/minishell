/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpirozzi <giovannipirozzi12345@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:22:51 by gpirozzi          #+#    #+#             */
/*   Updated: 2025/05/20 19:00:44 by gpirozzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.h"

/**
 * @brief Restores the previously duplicated standard input.
 *
 * Duplicates the saved file descriptor in `saved_stdin` back onto standard input (fd 0),
 * then closes the saved file descriptor.
 *
 * @param saved_stdin Pointer to the file descriptor to restore
 */
 void	restore_sdin(int *saved_stdin)
 {
	 dup2(*saved_stdin, 0);
	 close(*saved_stdin);
 }

 /**
  * @brief Handles input redirection from a file.
  *
  * Duplicates standard input and saves the duplicated fd into `saved_stdin`.
  * Opens the input file `input_file` for reading and duplicates its fd onto standard input.
  *
  * @param input_file Name of the input file
  * @param saved_stdin Pointer to store the original standard input fd
  * @return int 1 on success, 0 if input_file is NULL, -1 on error
  */
 int	handle_input_redirection(char *input_file, int	*saved_stdin)
 {
	 int	fd;

	 fd = 0;
	 if (!input_file)
		 return (0);
	 *saved_stdin = dup(0);
	 if (*saved_stdin < 0)
		 return (perror("dup"), -1);
	 fd = open(input_file, O_RDONLY);
	 if (fd == -1)
		 return (perror(input_file), close(*saved_stdin), -1);
	 if (dup2(fd, 0))
		 return (close(fd), perror("dup2"), close(*saved_stdin), -1);
	 return (close(fd), 1);
 }

 /**
  * @brief Adds a `pipe` element to the linked list of pipes.
  *
  * @param head Pointer to the pointer of the head of the pipe list
  * @param pipe New pipe element to add
  */
 void	add_pipe(t_pipe **head, t_pipe *pipe)
 {
	 t_pipe	*tmp;

	 tmp = NULL;
	 if (!head || !pipe)
		 return ;
	 if (*head == NULL)
		 *head = pipe;
	 else
	 {
		 tmp = *head;
		 while (tmp->next)
			 tmp = tmp->next;
		 tmp->next = pipe;
	 }
 }

 /**
  * @brief Checks if the token list contains a PIPE token.
  *
  * @param token Pointer to the head of the token list
  * @return true If a PIPE token is present in the list
  * @return false Otherwise
  */
 bool	has_pipe(t_token *token)
 {
	 while (token)
	 {
		 if (token->type == PIPE)
			 return (true);
		 token = token->next;
	 }
	 return (false);
 }

 /**
  * @brief Sets the pipe_flag to true for all commands in the list.
  *
  * @param head Pointer to the head of the command list
  */
 void	set_pipe_flag(t_cmd *head)
 {
	 t_cmd	*tmp;

	 tmp = head;
	 while (head)
	 {
		 head->pipe_flag = true;
		 head = head->next;
	 }
	 head = tmp;
 }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:04:29 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:27:38 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file shell_input.c
 * @brief Functions to manage shell input and heredoc data.
 *
 * This file provides access to a global shell_input structure,
 * functions to clear input, and manage heredoc input.
 */

#include <stdlib.h>

#include "shell.h"

/**
 * @brief Returns a globally accessible shell_input structure.
 *
 * Maintains a static t_shell_input instance shared across the program
 * and returns its address.
 *
 * @return t_shell_input* Pointer to the global shell_input structure.
 */
t_shell_input	*get_shell_input(void)
{
	static t_shell_input	shell_input;

	return (&shell_input);
}

/**
 * @brief Clears the shell_input structure.
 *
 * Frees any allocated input_line, token list, and syntax node
 * and resets the corresponding pointers to NULL.
 */
void	clear_shell_input(void)
{
	if ((get_shell_input())->input_line)
	{
		free((get_shell_input())->input_line);
		(get_shell_input())->input_line = NULL;
	}
	if ((get_shell_input())->input_token)
	{
		clear_token_lst(&((get_shell_input())->input_token));
		(get_shell_input())->input_token = NULL;
	}
	if ((get_shell_input())->input_node)
	{
		remove_syntax_node((get_shell_input())->input_node);
		(get_shell_input())->input_node = NULL;
	}
}

/**
 * @brief Turns off evaluation for the current input node.
 *
 * Delegates to turnoff_node_eval() for the input_node.
 */
void	turnoff_input_node_eval(void)
{
	turnoff_node_eval((get_shell_input())->input_node);
}

/**
 * @brief Returns a pointer to the heredoc input string.
 *
 * Maintains a static char* for heredoc input and returns its address.
 *
 * @return char** Pointer to the heredoc input string pointer.
 */
char	**get_heredoc_input(void)
{
	static char	*heredoc_input;

	return (&heredoc_input);
}

/**
 * @brief Clears the heredoc input.
 *
 * Frees the allocated heredoc input string and sets the pointer to NULL.
 */
void	clear_heredoc_input(void)
{
	char	**heredoc_input;

	heredoc_input = get_heredoc_input();
	if (*heredoc_input)
		free(*heredoc_input);
	*get_heredoc_input() = NULL;
}

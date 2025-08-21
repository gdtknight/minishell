/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:04:29 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 10:13:41 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"

t_shell_input	*get_shell_input(void)
{
	static t_shell_input	shell_input;

	return (&shell_input);
}

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

void	turnoff_input_node_eval(void)
{
	turnoff_node_eval((get_shell_input())->input_node);
}

char	**get_heredoc_input(void)
{
	static char	*heredoc_input;

	return (&heredoc_input);
}

void	clear_heredoc_input(void)
{
	char	**heredoc_input;

	heredoc_input = get_heredoc_input();
	if (*heredoc_input)
		free(*heredoc_input);
	*get_heredoc_input() = NULL;
}

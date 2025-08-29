/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:37:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 20:11:05 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file shell_signal_handler.c
 * @brief Signal handlers for minishell, pipelines, and heredoc.
 *
 * This file provides signal handlers for SIGINT (Ctrl-C) in different
 * contexts of the shell, including interactive mode, pipeline execution,
 * and heredoc input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

#include "libft.h"
#include "shell.h"

/**
 * @brief SIGINT (Ctrl-C) handler for interactive minishell.
 *
 * Cancels the current input line, moves to a new line, and redisplays
 * the prompt. Sets the shell's last_status to 128 + SIGINT.
 *
 * @param signo Signal number (SIGINT)
 *
 * @note
 * - Uses readline functions rl_replace_line(), rl_on_new_line(),
 *   and rl_redisplay() to update the input buffer.
 * - Writes a newline to STDERR to move the cursor to the next line.
 * - If not in heredoc mode, redisplays the prompt.
 */
void	minishell_sigint_handler(int signo)
{
	(void)signo;
	turnoff_input_node_eval();
	clear_heredoc_input();
	(get_shell_data())->last_status = 128 + SIGINT;
	if (!((get_shell_data())->in_heredoc)
		&& !((get_shell_data())->has_child))
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

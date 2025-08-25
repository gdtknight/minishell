/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:37:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:29:38 by yoshin           ###   ########.fr       */
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
	rl_replace_line("", 0);
	write(STDERR_FILENO, "\n", 1);
	if (!(get_shell_data())->in_heredoc)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

/**
 * @brief SIGINT (Ctrl-C) handler for pipeline processes.
 *
 * Cleans up shell input, heredoc input, and shell data, then exits
 * with status 128 + SIGINT.
 *
 * @param signo Signal number (SIGINT)
 */
void	pipeline_sigint_handler(int signo)
{
	(void)signo;
	clear_heredoc_input();
	clear_shell_input();
	clear_shell_data();
	exit(128 + SIGINT);
}

/**
 * @brief SIGINT (Ctrl-C) handler during heredoc input.
 *
 * Waits for any child process to terminate. If no child exists, writes
 * a newline. Then clears shell input, heredoc input, and shell data,
 * and exits with status 128 + SIGINT.
 *
 * @param signo Signal number (SIGINT)
 */
void	heredoc_sigint_handler(int signo)
{
	int	status;

	(void) signo;
	if (wait(&status) == -1)
		write(STDERR_FILENO, "\n", 1);
	clear_shell_input();
	clear_heredoc_input();
	clear_shell_data();
	exit (128 + SIGINT);
}

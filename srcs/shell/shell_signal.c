/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:17:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 23:22:24 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file shell_signal.c
 * @brief Signal handling for minishell, pipelines, and heredoc.
 *
 * This file provides initialization and restoration functions for
 * signal handlers used in different contexts of the shell.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>
#include <sys/wait.h>

#include "shell.h"

/**
 * @brief Initialize signals for interactive minishell.
 *
 * Saves old SIGINT and SIGQUIT actions and sets new handlers.
 * SIGINT is handled by minishell_sigint_handler.
 * SIGQUIT is ignored.
 */
void	init_minishell_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = minishell_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Initialize signals for heredoc input.
 *
 * Sets SIGINT to be handled by heredoc_sigint_handler.
 * SIGQUIT is ignored.
 */
void	init_heredoc_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Restore original signal handlers.
 *
 * Restores SIGINT and SIGQUIT actions saved in shell_data.
 */
void	restore_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

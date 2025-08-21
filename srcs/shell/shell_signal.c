/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:17:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 18:08:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	init_minishell_signal(void)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, &(get_shell_data()->old_int));
	sigaction(SIGQUIT, NULL, &(get_shell_data()->old_quit));
	sa.sa_handler = minishell_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	init_pipeline_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = pipeline_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGPIPE, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	init_heredoc_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signal(void)
{
	sigaction(SIGINT, &((get_shell_data())->old_int), NULL);
	sigaction(SIGQUIT, &((get_shell_data())->old_quit), NULL);
}

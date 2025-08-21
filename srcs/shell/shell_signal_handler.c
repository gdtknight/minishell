/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:37:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 10:04:05 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

#include "shell.h"

/**
 * @brief SIGINT(Ctrl-C) 시그널 핸들러
 *
 * 사용자가 Ctrl-C를 입력했을 때 현재 입력 줄을 취소하고,
 * 새 줄로 이동하여 프롬프트를 다시 표시한다.
 * 또한 종료 상태값(last_status)을 1로 설정한다.
 *
 * @param signo 전달된 시그널 번호 (SIGINT)
 *
 * @note
 * - rl_replace_line(), rl_on_new_line(), rl_redisplay()를 사용해 readline 버퍼를 갱신한다.
 * - 표준 출력(STDOUT)에 개행을 출력해 커서를 다음 줄로 내린다.
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

void	pipeline_sigint_handler(int signo)
{
	(void)signo;
	clear_shell_input();
	exit(128 + SIGINT);
}

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

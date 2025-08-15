/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:17:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:59:07 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

#include "shell_data.h"

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
static void	sigint_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(get_shell_data())->last_status = 1;
}

void	init_signals(void)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, &(get_shell_data()->old_int));
	sigaction(SIGQUIT, NULL, &(get_shell_data()->old_quit));
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals(void)
{
	sigaction(SIGINT, &((get_shell_data())->old_int), NULL);
	sigaction(SIGQUIT, &((get_shell_data())->old_quit), NULL);
}

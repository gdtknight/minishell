/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11                                     */
/*   Updated: 2025/08/11                                     */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

#include "shell_data.h"

/* ---------- Ctrl-C(SIGINT) 핸들러 ---------- */
static void sigint_handler(int signo)
{
    (void)signo;
    write(STDOUT_FILENO, "\n", 1);            // 현재 줄 내려주기
    rl_replace_line("", 0);                   // 입력 내용 삭제
    rl_on_new_line();                         // readline에 "새 줄" 상태 알림
    rl_redisplay();                            // 프롬프트 다시 렌더링
    (get_shell_data())->last_status = 1;       // 종료 상태값 업데이트
}

/* ---------- 시그널 초기화 ---------- */
void init_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // 인터럽트 후 재시작
    sigaction(SIGINT, &sa, NULL);

    signal(SIGQUIT, SIG_IGN); // Ctrl-\ 무시
}

/* ---------- 터미널 설정 저장 ---------- */
void save_terminal_settings(void)
{
    struct termios tio;

    tcgetattr(STDIN_FILENO, &tio);
    (get_shell_data())->termios_backup = tio;
}

/* ---------- 터미널 기본 모드 복원 ---------- */
void restore_terminal_settings(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW,
              &(get_shell_data())->termios_backup);
}

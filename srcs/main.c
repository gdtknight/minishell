/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:46:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 22:15:11 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "libft.h"

#include "shell_data.h"
#include "tokenizer.h"
#include "parser.h"
#include "eval.h"
#include "sig.h"

static void	interactive_mode(void);
static void	process_input(char *input);

/**
 * @file main.c
 * @brief minishell의 진입점 및 인터랙티브 모드 루프
 *
 * 프로그램 실행 시 터미널 상태를 저장하고, 시그널과 쉘 데이터를 초기화한다.
 * - argc == 1 → interactive_mode() 실행
 * - argc > 1 → (향후 구현 시) 스크립트 실행 가능
 *
 * @note
 * - 종료 시 restore_terminal_settings()로 터미널 상태 복구
 * - exit() 호출 시 get_shell_data()->last_status 반환
 */

/**
 * @brief 프로그램 진입점
 *
 * @param argc 실행 인자 개수
 * @param argv 실행 인자 배열
 * @param envp 환경 변수 배열
 * @return int 종료 상태 코드
 *
 * @details
 * - save_terminal_settings(): 현재 터미널 설정 저장
 * - init_signals(): SIGINT, SIGQUIT, SIGTERM 등 시그널 핸들러 등록
 * - init_shell_data(envp): 환경 변수 복사 및 내부 데이터 초기화
 * - interactive_mode(): 입력/파싱/실행 루프
 * - restore_terminal_settings(): 터미널 상태 복원
 */
int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	init_signals();
	save_terminal_settings();
	if (!init_shell_data(envp))
		exit(errno);
	if (argc == 1)
		interactive_mode();
	restore_terminal_settings();
	restore_signals();
	exit(get_shell_data()->last_status);
}

/**
 * @brief 인터랙티브 모드에서 사용자 입력을 읽고 처리하는 루프
 *
 * @return int 마지막 명령의 종료 상태
 *
 * @details
 * 1. readline(PROMPT)로 입력 받기
 * 2. 입력이 빈 문자열이면 무시
 * 3. add_history()로 히스토리에 저장
 * 4. tokenize_input()으로 토큰화
 * 5. is_valid_sequence()로 구문 유효성 검사
 * 6. parse_input() → 구문 트리 생성
 * 7. eval() → 구문 트리 실행
 * 8. 사용한 메모리 해제
 *
 * @note
 * - Ctrl+D 입력 시(NULL) 루프 종료
 * - last_status는 마지막 실행 결과로 갱신
 * - PROMPT는 전역 상수로, 쉘 프롬프트 문자열
 */
static void	interactive_mode(void)
{
	char			*input;

	while (TRUE)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input == '\0' || ft_strncmp(input, "", ft_strlen(input)) == 0)
			continue ;
		add_history(input);
		process_input(input);
	}
}

static void	process_input(char *input)
{
	t_token			*tk_lst;
	t_token			*tk;
	t_syntax_node	*syntax_tree;
	t_syntax_node	*st;

	tk_lst = tokenize_input(input);
	if (!is_valid_sequence(tk_lst))
		printf("Invalid input\n");
	else
	{
		tk = tk_lst;
		syntax_tree = parse_input(&tk);
		clear_token_lst(&tk_lst);
		st = syntax_tree;
		eval(st);
		remove_syntax_node(syntax_tree);
	}
	dup2(get_shell_data()->stdin_fd, STDIN_FILENO);
	dup2(get_shell_data()->stdout_fd, STDOUT_FILENO);
	clear_token_lst(&tk_lst);
}

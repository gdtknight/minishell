/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:45:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 08:44:11 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/_types/_pid_t.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include "def.h"

#include "ast.h"
#include "shell.h"
#include "eval.h"

static void	start_child(
				t_syntax_node *pipeline_node,
				pid_t child_pids[2],
				int pipe_fds[2],
				int left_or_right);
static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2]);
static void	wait_pipe(pid_t child_pids[2], int *status);

/**
 * @brief 파이프라인 노드를 평가하여 명령을 실행한다.
 *
 * 좌측 명령의 출력을 파이프를 통해 우측 명령의 입력으로 연결한다.
 * 두 개의 자식 프로세스를 fork()하여 각각 좌측/우측 명령을 실행하고,
 * 부모 프로세스는 파이프를 정리한 뒤 두 자식이 종료될 때까지 대기한다.
 *
 * @param pipeline_node 파이프라인을 나타내는 AST 노드
 * @return t_status 마지막 명령의 종료 상태 코드
 *
 * @note
 * - 좌/우측 자식 모두 restore_terminal_settings()를 호출해 터미널 상태를 복원한다.
 * - 자식 프로세스는 setup_pipe()를 호출해 입출력 리다이렉션을 설정한다.
 * - 부모 프로세스는 setup_pipe()로 모든 파이프 FD를 닫는다.
 * - 종료 상태는 wait_pipe()에서 갱신되며 get_shell_data()->last_status에 저장된다.
 */
void	eval_pipeline(t_syntax_node	*pipeline_node)
{
	t_status	status;
	int			pipe_fds[2];
	pid_t		child_pids[2];

	if (!pipeline_node || pipeline_node->eval == OFF)
		return ;
	get_shell_data()->in_pipe = TRUE;
	child_pids[CHILD_LEFT] = INT_MAX;
	child_pids[CHILD_RIGHT] = INT_MAX;
	if (pipe(pipe_fds) == -1)
	{
		perror("eval_pipeline");
		get_shell_data()->last_status = EXIT_FAILURE;
		return ;
	}
	child_pids[CHILD_LEFT] = fork();
	if (child_pids[CHILD_LEFT] == 0)
		start_child(pipeline_node, child_pids, pipe_fds, CHILD_LEFT);
	child_pids[CHILD_RIGHT] = fork();
	if (child_pids[CHILD_RIGHT] == 0)
		start_child(pipeline_node, child_pids, pipe_fds, CHILD_RIGHT);
	setup_pipe(child_pids, pipe_fds);
	wait_pipe(child_pids, &status);
	get_shell_data()->in_pipe = FALSE;
}

static void	start_child(
				t_syntax_node *pipeline_node,
				pid_t child_pids[2],
				int pipe_fds[2],
				int left_or_right)
{
	if (left_or_right == CHILD_LEFT)
	{
		setup_pipe(child_pids, pipe_fds);
		init_pipeline_signal();
		eval(pipeline_node->value.b_node.left);
		exit(get_shell_data()->last_status);
	}
	else
	{
		setup_pipe(child_pids, pipe_fds);
		init_pipeline_signal();
		eval(pipeline_node->value.b_node.right);
		exit(get_shell_data()->last_status);
	}
}

/**
 * @brief 파이프라인 실행 시 각 프로세스의 입출력을 설정한다.
 *
 * CHILD_LEFT 프로세스는 STDOUT을 파이프 쓰기 끝에 연결하고,
 * CHILD_RIGHT 프로세스는 STDIN을 파이프 읽기 끝에 연결한다.
 * 부모 프로세스는 파이프의 양쪽 끝을 모두 닫는다.
 *
 * @param child_pids 자식 PID 배열 (CHILD_LEFT, CHILD_RIGHT)
 * @param pipe_fds   파이프 파일 디스크립터 배열 (PIPE_READ, PIPE_WRITE)
 *
 * @note
 * - dup2() 호출 후 원래 FD는 반드시 close()로 닫아준다.
 * - 부모 프로세스에서는 사용하지 않는 파이프 FD를 모두 닫아 리소스를 해제한다.
 */
static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2])
{
	if (child_pids[CHILD_RIGHT] == 0)
	{
		close(STDIN_FILENO);
		close(pipe_fds[PIPE_WRITE]);
		dup2(pipe_fds[PIPE_READ], STDIN_FILENO);
		return ;
	}
	if (child_pids[CHILD_LEFT] == 0)
	{
		close(STDOUT_FILENO);
		close(pipe_fds[PIPE_READ]);
		dup2(pipe_fds[PIPE_WRITE], STDOUT_FILENO);
		return ;
	}
	close(pipe_fds[PIPE_READ]);
	close(pipe_fds[PIPE_WRITE]);
}

/**
 * @brief 파이프라인의 두 자식 프로세스를 기다린다.
 *
 * 좌측/우측 자식 프로세스가 종료될 때까지 waitpid()로 대기하고,
 * 종료 상태를 분석하여 get_shell_data()->last_status에 저장한다.
 *
 * @param child_pids 자식 PID 배열 (CHILD_LEFT, CHILD_RIGHT)
 * @param status     마지막 waitpid()의 상태 코드를 저장할 포인터
 * @return int 마지막으로 대기한 프로세스의 상태 코드
 *
 * @note
 * - 정상 종료 시 WEXITSTATUS를, 시그널 종료 시 WTERMSIG를 last_status에 기록한다.
 */
static void	wait_pipe(pid_t child_pids[2], int *status)
{
	pid_t	child;
	int		sig;

	sig = 0;
	child = waitpid(-1, status, 0);
	if (child == child_pids[CHILD_LEFT])
	{
		if (WIFSIGNALED(*status))
		{
			sig = WTERMSIG(*status);
			kill(child_pids[CHILD_RIGHT], SIGTERM);
			(get_shell_data())->last_status = WTERMSIG(*status);
			return ;
		}
		wait_child(child_pids[CHILD_RIGHT], status, 0);
		return ;
	}
	(void)sig;
	wait_child(child_pids[CHILD_LEFT], status, 0);
}

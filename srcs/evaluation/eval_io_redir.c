/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:39:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 06:46:44 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>

#include "libft.h"

#include "def.h"
#include "ast.h"
#include "shell.h"
#include "eval.h"

static t_status	set_stdin(t_syntax_node *io_redir_node);
static t_status	set_heredoc(t_syntax_node *io_redir_node);
static t_status	set_stdout(t_syntax_node *io_redir_node);

/**
 * @brief I/O 리다이렉션 노드를 평가하여 입출력 스트림을 재설정한다.
 *
 * 노드 타입에 따라 입력(STDIN) 또는 출력(STDOUT) 리다이렉션을 설정한다.
 * - NODE_IO_REDIR_IN / NODE_IO_REDIR_HEREDOC → set_stdin()
 * - NODE_IO_REDIR_OUT / NODE_IO_REDIR_APPEND → set_stdout()
 *
 * @param io_redir_node I/O 리다이렉션을 나타내는 구문 노드
 * @return t_status SUCCESS(0) 또는 ERROR(비0)
 *
 * @note
 * - NULL 노드 입력 시 아무 동작 없이 SUCCESS 반환.
 * - 각 하위 함수는 dup2()를 사용하여 표준 입출력을 새로운 FD로 교체한다.
 */
t_status	eval_io_redir(t_syntax_node *io_redir_node)
{
	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	if (io_redir_node->type == NODE_IO_REDIR_IN
		|| io_redir_node->type == NODE_IO_REDIR_HEREDOC)
		return (set_stdin(io_redir_node));
	else if (io_redir_node->type == NODE_IO_REDIR_OUT
		|| io_redir_node->type == NODE_IO_REDIR_APPEND)
		return (set_stdout(io_redir_node));
	return (SUCCESS);
}

/**
 * @brief 표준 입력(STDIN)을 재설정한다.
 *
 * - NODE_IO_REDIR_IN: 지정된 파일을 O_RDONLY로 열어 STDIN에 연결
 * - NODE_IO_REDIR_HEREDOC: set_heredoc()으로 처리
 *
 * @param io_redir_node 입력 리다이렉션 노드
 * @return t_status SUCCESS 또는 ERROR
 *
 * @note
 * - open() 실패 시 errno 메시지를 perror()로 출력하고 ERROR 반환.
 * - dup2() 이후 원본 FD는 닫지 않고 반환(heredoc 제외).
 */
static t_status	set_stdin(t_syntax_node *io_redir_node)
{
	int		infile_fd;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	if (io_redir_node->type == NODE_IO_REDIR_IN)
	{
		infile_fd = open(io_redir_node->value.io_target, O_RDONLY);
		if (infile_fd == -1)
		{
			perror(io_redir_node->value.io_target);
			return (FAILURE);
		}
		close(STDIN_FILENO);
		dup2(infile_fd, STDIN_FILENO);
		return (SUCCESS);
	}
	return (set_heredoc(io_redir_node));
}

/**
 * @brief heredoc(<<) 입력을 설정한다.
 *
 * 파이프를 생성하고, 자식 프로세스에서 heredoc 내용을 작성하여
 * 부모 프로세스의 STDIN으로 연결한다.
 *
 * @param io_redir_node heredoc 리다이렉션 노드
 * @return t_status 항상 SUCCESS (실패 시 ERROR 반환 가능)
 *
 * @note
 * - heredoc() 함수로 내용 생성 → 파이프 쓰기 → 부모에서 파이프 읽기 FD를 STDIN에 연결
 * - 자식 종료 상태를 get_shell_data()->last_status에 반영
 */
static t_status	set_heredoc(t_syntax_node *io_redir_node)
{
	t_syntax_node	*cmd;
	int		status;
	pid_t	child;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	cmd = io_redir_node;
	while (cmd->type != NODE_SIMPLE_COMMAND)
		cmd = cmd->parent;
	(get_shell_data())->in_heredoc = TRUE;
	if (cmd->value.command.heredoc_fds[PIPE_READ] != -1)
		close(cmd->value.command.heredoc_fds[PIPE_READ]);
	if (pipe(cmd->value.command.heredoc_fds) == -1)
	{
		perror("heredoc_fds");
		exit(errno);
	}
	child = fork();
	if (child == 0)
	{
		close((cmd->value.command.heredoc_fds)[PIPE_READ]);
		ft_putstr_fd(
			(*(get_heredoc_input())),
			(cmd->value.command.heredoc_fds)[PIPE_WRITE]);
		close((cmd->value.command.heredoc_fds)[PIPE_WRITE]);
		clear_shell_input();
		clear_shell_data();
		clear_heredoc_input();
		exit(EXIT_SUCCESS);
	}
	close((cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	dup2((cmd->value.command.heredoc_fds)[PIPE_READ], STDIN_FILENO);
	close((cmd->value.command.heredoc_fds)[PIPE_READ]);
	waitpid(child, &status, 0);
	(get_shell_data())->in_heredoc = FALSE;
	return (SUCCESS);
}

/**
 * @brief 표준 출력(STDOUT)을 재설정한다.
 *
 * - NODE_IO_REDIR_OUT: 쓰기 전용, 없으면 생성(O_CREAT), 기존 내용 삭제(O_TRUNC)
 * - NODE_IO_REDIR_APPEND: 쓰기 전용, 없으면 생성(O_CREAT), 기존 내용 뒤에 추가(O_APPEND)
 *
 * @param io_redir_node 출력 리다이렉션 노드
 * @return t_status SUCCESS 또는 ERROR
 *
 * @note
 * - open() 실패 시 errno 메시지를 perror()로 출력하고 ERROR 반환.
 * - 성공 시 dup2()로 STDOUT을 새 FD에 연결하고 원본 FD는 닫는다.
 */
static t_status	set_stdout(t_syntax_node *io_redir_node)
{
	t_status	outfile_fd;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	if (io_redir_node->type == NODE_IO_REDIR_OUT)
	{
		outfile_fd = open(io_redir_node->value.io_target, \
			O_RDWR | O_CREAT | O_TRUNC, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	}
	else
	{
		outfile_fd = open(io_redir_node->value.io_target, \
			O_RDWR | O_CREAT | O_APPEND, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	}
	if (outfile_fd == -1)
	{
		perror(io_redir_node->value.io_target);
		return (FAILURE);
	}
	close(STDOUT_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	return (SUCCESS);
}

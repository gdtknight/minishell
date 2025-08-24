/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:21:43 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 12:38:42 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "ast.h"
#include "libft.h"

#include "def.h"
#include "shell.h"
#include "eval.h"

static void				set_heredoc_to_pipe(
							t_syntax_node *cmd,
							t_syntax_node *io_redir_node);
static void				set_heredoc_from_pipe(
							t_syntax_node *cmd,
							pid_t child);

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
t_status	set_stdin(t_syntax_node *io_redir_node)
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
t_status	set_heredoc(t_syntax_node *io_redir_node)
{
	t_syntax_node	*cmd;
	pid_t			child;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	(get_shell_data())->in_heredoc = TRUE;
	cmd = find_cmd_node(io_redir_node);
	if (!cmd)
		return (SUCCESS);
	if (pipe(cmd->value.command.heredoc_fds) == -1)
	{
		perror("heredoc_fds");
		exit(errno);
	}
	child = fork();
	if (child == 0)
		set_heredoc_to_pipe(cmd, io_redir_node);
	set_heredoc_from_pipe(cmd, child);
	(get_shell_data())->in_heredoc = FALSE;
	return (SUCCESS);
}


static void	set_heredoc_to_pipe(t_syntax_node *cmd, t_syntax_node *io_redir_node)
{
	close((cmd->value.command.heredoc_fds)[PIPE_READ]);
	ft_putstr_fd(
		io_redir_node->value.io_target,
		(cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	close((cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	clear_shell_input();
	clear_shell_data();
	clear_heredoc_input();
	exit(EXIT_SUCCESS);
}

static void	set_heredoc_from_pipe(t_syntax_node *cmd, pid_t child)
{
	int	status;

	close((cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	dup2((cmd->value.command.heredoc_fds)[PIPE_READ], STDIN_FILENO);
	close((cmd->value.command.heredoc_fds)[PIPE_READ]);
	waitpid(child, &status, 0);
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
t_status	set_stdout(t_syntax_node *io_redir_node)
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

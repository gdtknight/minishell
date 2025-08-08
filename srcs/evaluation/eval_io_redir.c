/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:39:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 21:32:38 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft.h"

#include "def.h"

#include "eval.h"
#include "parser.h"
#include "shell_data.h"
#include "utils.h"

static t_status	set_stdin(t_syntax_node *io_redir_node);
static t_status	set_heredoc(t_syntax_node *io_redir_node);
static t_status	set_stdout(t_syntax_node *io_redir_node);

t_status	eval_io_redir(t_syntax_node *io_redir_node)
{
	t_status	status;

	status = 0;
	if (!io_redir_node)
		return (status);
	if (io_redir_node->type == NODE_IO_REDIR_IN
		|| io_redir_node->type == NODE_IO_REDIR_HEREDOC)
		status = set_stdin(io_redir_node);
	else if (io_redir_node->type == NODE_IO_REDIR_OUT
		|| io_redir_node->type == NODE_IO_REDIR_APPEND)
		status = set_stdout(io_redir_node);
	return (status);
}

static t_status	set_stdin(t_syntax_node *io_redir_node)
{
	int		infile_fd;

	if (io_redir_node->type == NODE_IO_REDIR_IN)
	{
		infile_fd = open(io_redir_node->value.io_target, O_RDONLY);
		if (infile_fd == -1)
		{
			perror(strerror(errno));
			return (ERROR);
		}
		close(STDIN_FILENO);
		dup2(infile_fd, STDIN_FILENO);
		return (SUCCESS);
	}
	return (set_heredoc(io_redir_node));
}

static t_status	set_heredoc(t_syntax_node *io_redir_node)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fds) == -1)
		return (ERROR);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fds[PIPE_READ]);
		ft_putstr_fd(
			heredoc(io_redir_node->value.io_target),
			pipe_fds[PIPE_WRITE]);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fds[PIPE_WRITE]);
	close(STDIN_FILENO);
	dup2(pipe_fds[PIPE_READ], STDIN_FILENO);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(get_shell_data())->last_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		(get_shell_data())->last_status = WTERMSIG(status);
	return (SUCCESS);
}

static t_status	set_stdout(t_syntax_node *io_redir_node)
{
	t_status	outfile_fd;

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
		perror(strerror(errno));
		return (ERROR);
	}
	close(STDOUT_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:21:43 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 00:48:46 by yoshin           ###   ########.fr       */
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
 * @brief Sets up stdin redirection from a file or heredoc.
 *
 * Opens the input file or calls set_heredoc for heredoc nodes.
 *
 * @param io_redir_node Pointer to the I/O redirection node.
 * @return t_status SUCCESS or FAILURE.
 */
t_status	set_stdin(t_syntax_node *io_redir_node)
{
	int		infile_fd;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	(get_shell_data())->is_redir = TRUE;
	if (io_redir_node->type == NODE_IO_REDIR_IN)
	{
		infile_fd = open(io_redir_node->value.io_target, O_RDONLY);
		if (infile_fd == -1)
		{
			perror(io_redir_node->value.io_target);
			return (ERROR);
		}
		close(STDIN_FILENO);
		dup2(infile_fd, STDIN_FILENO);
		return (SUCCESS);
	}
	return (set_heredoc(io_redir_node));
}

/**
 * @brief Sets up heredoc redirection for a command node.
 *
 * Creates a pipe, forks a child to write heredoc data, and sets up the
 * parent to read from the pipe.
 *
 * @param io_redir_node Pointer to the heredoc redirection node.
 * @return t_status SUCCESS.
 */
t_status	set_heredoc(t_syntax_node *io_redir_node)
{
	t_syntax_node	*cmd;
	pid_t			child;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
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
	return (SUCCESS);
}

/**
 * @brief Child process: writes heredoc data to the pipe.
 *
 * Writes the heredoc input to the pipe and exits.
 *
 * @param cmd Pointer to the command node.
 * @param io_redir_node Pointer to the heredoc redirection node.
 */
static void	set_heredoc_to_pipe(
				t_syntax_node *cmd,
				t_syntax_node *io_redir_node)
{
	close((cmd->value.command.heredoc_fds)[PIPE_READ]);
	ft_putstr_fd(
		io_redir_node->value.io_target,
		(cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	close((cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	clear_shell_input();
	clear_shell_data();
	exit(EXIT_SUCCESS);
}

/**
 * @brief Parent process: sets up stdin from heredoc pipe and waits for child.
 *
 * Duplicates the pipe to stdin and waits for the child process to finish.
 *
 * @param cmd Pointer to the command node.
 * @param child PID of the child process.
 */
static void	set_heredoc_from_pipe(t_syntax_node *cmd, pid_t child)
{
	int	status;

	close((cmd->value.command.heredoc_fds)[PIPE_WRITE]);
	dup2((cmd->value.command.heredoc_fds)[PIPE_READ], STDIN_FILENO);
	close((cmd->value.command.heredoc_fds)[PIPE_READ]);
	waitpid(child, &status, 0);
}

/**
 * @brief Sets up stdout redirection to a file.
 *
 * Opens the output file (truncate or append) and duplicates it to stdout.
 *
 * @param io_redir_node Pointer to the I/O redirection node.
 * @return t_status SUCCESS or FAILURE.
 */
t_status	set_stdout(t_syntax_node *io_redir_node)
{
	t_status	outfile_fd;

	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	(get_shell_data())->is_redir = TRUE;
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

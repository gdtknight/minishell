/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:19:13 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 20:10:08 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <unistd.h>

#include "libft.h"

#include "shell.h"
#include "utils.h"
#include "eval.h"

static void	start_heredoc(int heredoc_pipe[2], t_syntax_node *node);
static void	receive_heredoc(pid_t child_pid, int heredoc_pipe[2]);
static void	read_input(const char *limiter, int heredoc_pipe[2]);
static void	read_heredoc_pipe(int pipe_fd);

/**
 * @brief Reads heredoc input for a given syntax node.
 *
 * Sets up a pipe and forks a child to collect heredoc input, then stores
 * the result in the node.
 *
 * @param node Pointer to the syntax node for heredoc.
 */
void	read_heredoc(t_syntax_node *node)
{
	int		heredoc_pipe[2];
	pid_t	child_pid;
	char	*temp;

	if (!node)
		return ;
	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	get_shell_data()->has_child = TRUE;
	if (child_pid == 0)
		start_heredoc(heredoc_pipe, node);
	receive_heredoc(child_pid, heredoc_pipe);
	get_shell_data()->has_child = FALSE;
	temp = node->value.io_target;
	node->value.io_target = ft_strdup(*get_heredoc_input());
	free(temp);
	clear_heredoc_input();
}

/**
 * @brief Child process: collects heredoc input and writes to pipe.
 *
 * Initializes heredoc signal, reads input until limiter, writes to pipe,
 * and exits.
 *
 * @param heredoc_pipe Pipe file descriptors.
 * @param node Pointer to the syntax node for heredoc.
 */
static void	start_heredoc(int heredoc_pipe[2], t_syntax_node *node)
{
	init_heredoc_signal();
	clear_heredoc_input();
	close(heredoc_pipe[PIPE_READ]);
	read_input(node->value.io_target, heredoc_pipe);
	close(heredoc_pipe[PIPE_WRITE]);
	clear_heredoc_input();
	clear_shell_input();
	clear_shell_data();
	exit(EXIT_SUCCESS);
}

/**
 * @brief Reads heredoc input from the user until the limiter is reached.
 *
 * Appends each line to the heredoc input buffer.
 *
 * @param limiter The heredoc delimiter string.
 */
static void	read_input(const char *limiter, int heredoc_pipe[2])
{
	char	*input[3];

	input[0] = NULL;
	input[1] = NULL;
	input[2] = NULL;
	if (!limiter)
		return ;
	restore_tty();
	while (TRUE)
	{
		input[1] = readline("heredoc> ");
		if (!input[1]
			|| ft_strncmp(input[1], limiter, ft_strlen(limiter) + 1) == 0)
			break ;
		input[2] = input[0];
		if (input[2])
			input[0] = ft_multiplejoin(input[2], input[1], "\n");
		else
			input[0] = ft_strjoin(input[1], "\n");
		free(input[1]);
		free(input[2]);
	}
	ft_putstr_fd(input[0], heredoc_pipe[PIPE_WRITE]);
	free(input[0]);
}

/**
 * @brief Parent process: receives heredoc from child and updates shell state.
 *
 * Waits for the child, handles signals, and reads heredoc data from pipe.
 *
 * @param child_pid PID of the child process.
 * @param heredoc_pipe Pipe file descriptors.
 */
static void	receive_heredoc(pid_t child_pid, int heredoc_pipe[2])
{
	int	status;

	status = 0;
	(get_shell_data())->in_heredoc = TRUE;
	close(heredoc_pipe[PIPE_WRITE]);
	waitpid(child_pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(heredoc_pipe[PIPE_READ]);
		(get_shell_data())->last_status = 128 + SIGINT;
		if (WTERMSIG(status) == SIGINT)
			ft_putendl_fd("", STDERR_FILENO);
		turnoff_input_node_eval();
	}
	if (WIFEXITED(status))
	{
		clear_heredoc_input();
		read_heredoc_pipe(heredoc_pipe[PIPE_READ]);
		close(heredoc_pipe[PIPE_READ]);
		(get_shell_data())->last_status = WEXITSTATUS(status);
	}
	(get_shell_data())->in_heredoc = FALSE;
}

/**
 * @brief Reads heredoc data from a pipe and stores it in the heredoc buffer.
 *
 * Reads all data from the pipe and appends it to the heredoc input buffer.
 *
 * @param pipe_fd File descriptor to read from.
 */
static void	read_heredoc_pipe(int pipe_fd)
{
	char	*read_;
	char	*temp;
	int		read_byte;
	char	buf[256];

	read_ = NULL;
	buf[0] = '\0';
	read_byte = read(pipe_fd, buf, 255);
	buf[read_byte] = '\0';
	while (read_byte > 0)
	{
		read_ = ft_substr(buf, 0, read_byte);
		temp = *(get_heredoc_input());
		*(get_heredoc_input()) = ft_strjoin(temp, read_);
		free(temp);
		free(read_);
		read_byte = read(pipe_fd, buf, 255);
		buf[read_byte] = '\0';
	}
}

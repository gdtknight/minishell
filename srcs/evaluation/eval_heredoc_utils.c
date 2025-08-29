/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:19:13 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 07:35:01 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <unistd.h>

#include "ast.h"
#include "expand.h"
#include "get_next_line_bonus.h"
#include "libft.h"

#include "shell.h"
#include "eval.h"

static void	start_heredoc(int heredoc_pipe[2], t_syntax_node *node);
static void	receive_heredoc(
				pid_t child_pid,
				int heredoc_pipe[2],
				t_syntax_node *node);
static void	read_input(const char *limiter, int heredoc_pipe[2]);
static void	read_heredoc_pipe(int pipe_fd, t_syntax_node *node);

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
	receive_heredoc(child_pid, heredoc_pipe, node);
	get_shell_data()->has_child = FALSE;
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
	close(heredoc_pipe[PIPE_READ]);
	read_input(node->value.io_target, heredoc_pipe);
	close(heredoc_pipe[PIPE_WRITE]);
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
	char	*input;

	input = NULL;
	if (!limiter)
		return ;
	restore_tty();
	while (TRUE)
	{
		input = readline("heredoc> ");
		if (ft_strncmp(input, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(input);
			return ;
		}
		expand_heredoc_input(&input);
		ft_putstr_fd(input, heredoc_pipe[PIPE_WRITE]);
		free(input);
		ft_putstr_fd("\n", heredoc_pipe[PIPE_WRITE]);
	}
}

/**
 * @brief Parent process: receives heredoc from child and updates shell state.
 *
 * Waits for the child, handles signals, and reads heredoc data from pipe.
 *
 * @param child_pid PID of the child process.
 * @param heredoc_pipe Pipe file descriptors.
 */
static void	receive_heredoc(
				pid_t child_pid,
				int heredoc_pipe[2],
				t_syntax_node *node)
{
	int	status;

	status = 0;
	close(heredoc_pipe[PIPE_WRITE]);
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
	{
		(get_shell_data())->last_status = WEXITSTATUS(status);
		read_heredoc_pipe(heredoc_pipe[PIPE_READ], node);
		close(heredoc_pipe[PIPE_READ]);
	}
}

/**
 * @brief Reads heredoc data from a pipe and stores it in the heredoc buffer.
 *
 * Reads all data from the pipe and appends it to the heredoc input buffer.
 *
 * @param pipe_fd File descriptor to read from.
 */
static void	read_heredoc_pipe(int pipe_fd, t_syntax_node *node)
{
	char	*read_;
	char	*temp;

	free(node->value.io_target);
	node->value.io_target = NULL;
	read_ = get_next_line(pipe_fd);
	while (read_)
	{
		temp = node->value.io_target;
		if (temp)
		{
			node->value.io_target = ft_strjoin(temp, read_);
			free(temp);
		}
		else
			node->value.io_target = ft_strdup(read_);
		free(read_);
		read_ = get_next_line(pipe_fd);
	}
}

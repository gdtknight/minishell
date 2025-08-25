/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:45:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:08:06 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
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
 * @brief Evaluates a pipeline node by forking and setting up pipes.
 *
 * Forks two child processes for the left and right commands, sets up the
 * pipe, and waits for both children to finish.
 *
 * @param pipeline_node Pointer to the pipeline syntax node.
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

/**
 * @brief Starts a child process for a pipeline command.
 *
 * Sets up the pipe, initializes signals, evaluates the left or right node,
 * and exits.
 *
 * @param pipeline_node Pointer to the pipeline syntax node.
 * @param child_pids Array of child PIDs.
 * @param pipe_fds Pipe file descriptors.
 * @param left_or_right Indicates left or right child.
 */
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
		clear_heredoc_input();
		clear_shell_input();
		clear_shell_data();
		exit(get_shell_data()->last_status);
	}
	else
	{
		setup_pipe(child_pids, pipe_fds);
		init_pipeline_signal();
		eval(pipeline_node->value.b_node.right);
		clear_heredoc_input();
		clear_shell_input();
		clear_shell_data();
		exit(get_shell_data()->last_status);
	}
}

/**
 * @brief Sets up pipe file descriptors for the pipeline.
 *
 * Duplicates the pipe ends to stdin or stdout as needed for each child.
 *
 * @param child_pids Array of child PIDs.
 * @param pipe_fds Pipe file descriptors.
 */
static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2])
{
	if (child_pids[CHILD_RIGHT] == 0)
	{
		close(STDIN_FILENO);
		close(pipe_fds[PIPE_WRITE]);
		dup2(pipe_fds[PIPE_READ], STDIN_FILENO);
		close(pipe_fds[PIPE_READ]);
		return ;
	}
	if (child_pids[CHILD_LEFT] == 0)
	{
		close(STDOUT_FILENO);
		close(pipe_fds[PIPE_READ]);
		dup2(pipe_fds[PIPE_WRITE], STDOUT_FILENO);
		close(pipe_fds[PIPE_WRITE]);
		return ;
	}
	close(pipe_fds[PIPE_READ]);
	close(pipe_fds[PIPE_WRITE]);
}

/**
 * @brief Waits for both children in the pipeline to finish.
 *
 * Updates shell status and turns off node evaluation as needed.
 *
 * @param child_pids Array of child PIDs.
 * @param status Pointer to the status variable.
 */
static void	wait_pipe(pid_t child_pids[2], int *status)
{
	pid_t	child;

	child = waitpid(-1, status, 0);
	if (child == child_pids[CHILD_LEFT])
	{
		if (WIFSIGNALED(*status))
			(get_shell_data())->last_status = 128 + WTERMSIG(*status);
		wait_child(child_pids[CHILD_RIGHT], status, 0);
		clear_heredoc_input();
		turnoff_node_eval(get_shell_input()->input_node);
		return ;
	}
	wait_child(child_pids[CHILD_LEFT], status, 0);
	clear_heredoc_input();
	turnoff_node_eval(get_shell_input()->input_node);
}

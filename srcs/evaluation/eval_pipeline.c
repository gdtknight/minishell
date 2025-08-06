/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:45:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/06 18:16:07 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <wait.h>

#include "eval.h"

static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2]);
static int	wait_pipe(pid_t child_pids[2], int *status);

int	eval_pipeline(t_syntax_node	*pipeline_node)
{
	int		status;
	int		pipe_fds[2];
	pid_t	child_pids[2];

	child_pids[CHILD_LEFT] = INT_MAX;
	child_pids[CHILD_RIGHT] = INT_MAX;
	if (pipe(pipe_fds) == -1)
		exit(EXIT_FAILURE);
	child_pids[CHILD_LEFT] = fork();
	if (child_pids[CHILD_LEFT] == 0)
	{
		setup_pipe(child_pids, pipe_fds);
		exit(eval(pipeline_node->value.b_node.left));
	}
	child_pids[CHILD_RIGHT] = fork();
	if (child_pids[CHILD_RIGHT] == 0)
	{
		setup_pipe(child_pids, pipe_fds);
		exit(eval(pipeline_node->value.b_node.right));
	}
	setup_pipe(child_pids, pipe_fds);
	return (wait_pipe(child_pids, &status));
}

static int	wait_pipe(pid_t child_pids[2], int *status)
{
	pid_t	exited_child;

	exited_child = waitpid(-1, status, 0);
	if (exited_child == child_pids[CHILD_LEFT])
		waitpid(child_pids[CHILD_RIGHT], status, 0);
	else
		waitpid(child_pids[CHILD_LEFT], status, 0);
	return (*status);
}

static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2])
{
	if (child_pids[CHILD_LEFT] != 0 && child_pids[CHILD_RIGHT] != 0)
	{
		close(pipe_fds[PIPE_READ]);
		close(pipe_fds[PIPE_WRITE]);
	}
	else if (child_pids[CHILD_LEFT] == 0 && child_pids[CHILD_RIGHT] != 0)
	{
		close(STDOUT_FILENO);
		close(pipe_fds[PIPE_READ]);
		dup2(pipe_fds[PIPE_WRITE], STDOUT_FILENO);
	}
	else if (child_pids[CHILD_LEFT] != 0 && child_pids[CHILD_RIGHT] == 0)
	{
		close(STDIN_FILENO);
		close(pipe_fds[PIPE_WRITE]);
		dup2(pipe_fds[PIPE_READ], STDIN_FILENO);
	}
}

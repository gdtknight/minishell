/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:45:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 19:05:32 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "eval.h"
#include "shell_data.h"
#include "sig.h"

static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2]);
static int	wait_pipe(pid_t child_pids[2], int *status);

t_status	eval_pipeline(t_syntax_node	*pipeline_node)
{
	t_status	status;
	int			pipe_fds[2];
	pid_t		child_pids[2];

	child_pids[CHILD_LEFT] = INT_MAX;
	child_pids[CHILD_RIGHT] = INT_MAX;
	if (pipe(pipe_fds) == -1)
		exit(EXIT_FAILURE);
	child_pids[CHILD_LEFT] = fork();
	if (child_pids[CHILD_LEFT] == 0)
	{
		restore_terminal_settings();
		setup_pipe(child_pids, pipe_fds);
		exit(eval(pipeline_node->value.b_node.left));
	}
	child_pids[CHILD_RIGHT] = fork();
	if (child_pids[CHILD_RIGHT] == 0)
	{
		restore_terminal_settings();
		setup_pipe(child_pids, pipe_fds);
		exit(eval(pipeline_node->value.b_node.right));
	}
	setup_pipe(child_pids, pipe_fds);
	return (wait_pipe(child_pids, &status));
}

static int	wait_pipe(pid_t child_pids[2], int *status)
{
	waitpid(child_pids[0], status, 0);
	waitpid(child_pids[1], status, 0);
	if (WIFEXITED(*status))
		(get_shell_data())->last_status = WEXITSTATUS(*status);
	if (WIFSIGNALED(*status))
		(get_shell_data())->last_status = WTERMSIG(*status);
	return (*status);
}

static void	setup_pipe(pid_t child_pids[2], int pipe_fds[2])
{
	if (child_pids[CHILD_LEFT] == 0)
	{
		close(STDOUT_FILENO);
		close(pipe_fds[PIPE_READ]);
		dup2(pipe_fds[PIPE_WRITE], STDOUT_FILENO);
		close(pipe_fds[PIPE_WRITE]);
	}
	else if (child_pids[CHILD_RIGHT] == 0)
	{
		close(STDIN_FILENO);
		close(pipe_fds[PIPE_WRITE]);
		dup2(pipe_fds[PIPE_READ], STDIN_FILENO);
		close(pipe_fds[PIPE_READ]);
	}
	else
	{
		close(pipe_fds[PIPE_READ]);
		close(pipe_fds[PIPE_WRITE]);
	}
}

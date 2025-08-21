/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:57:31 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 18:12:02 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <sys/wait.h>

#include "libft.h"

#include "ast.h"
#include "eval.h"

#include "debug.h"
#include "utils.h"

static void	read_heredoc(t_syntax_node *node);
static void	heredoc(int	heredoc_pipe[2], t_syntax_node *node);
static void	read_input(char **input, const char *limiter);
static char	*read_from_pipe(int pipe_fd);

void	eval_heredoc(t_syntax_node *node)
{
	if (!node)
		return ;
	if ((node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND)
		|| (node->type == NODE_AND_IF || node->type == NODE_OR_IF)
		|| (node->type == NODE_PIPELINE || node->type == NODE_PIPELINE_ERR))
	{
		eval_heredoc(node->value.b_node.left);
		eval_heredoc(node->value.b_node.right);
	}
	else if (node->type == NODE_COMPOUND_COMMAND)
		eval_heredoc(node->value.child);
	else if (node->type == NODE_SIMPLE_COMMAND)
	{
		eval_heredoc(node->value.command.prefix);
		eval_heredoc(node->value.command.suffix);
	}
	else if ((node->type == NODE_CMD_PREFIX)
		|| (node->type == NODE_CMD_PREFIX))
	{
		eval_heredoc(node->value.b_node.left);
		eval_heredoc(node->value.b_node.right);
	}
	else if (node->type == NODE_IO_REDIR_HEREDOC)
		read_heredoc(node);
}

static void	read_heredoc(t_syntax_node *node)
{
	int		heredoc_pipe[2];
	pid_t	child_pid;
	char	*input;
	int		status;

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	if (child_pid == 0)
	{
		init_heredoc_signal();
		heredoc(heredoc_pipe, node);
	}
	close(heredoc_pipe[PIPE_WRITE]);
	(get_shell_data())->in_heredoc = TRUE;
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
	{
		(get_shell_data())->last_status = WEXITSTATUS(status);
		input = read_from_pipe(heredoc_pipe[PIPE_READ]);
		free(node->value.io_target);
		node->value.io_target = input;
	}
	if (WIFSIGNALED(status))
	{
		(get_shell_data())->last_status = 128 + WTERMSIG(status);
		clear_shell_input();
	}
	(get_shell_data())->in_heredoc = FALSE;
}

static void	heredoc(int	heredoc_pipe[2], t_syntax_node *node)
{
	char	*input;

	input = NULL;
	close(heredoc_pipe[PIPE_READ]);
	read_input(&input, node->value.io_target);
	close(heredoc_pipe[PIPE_WRITE]);
	ft_putstr_fd(input, heredoc_pipe[PIPE_WRITE]);
	clear_shell_input();
	clear_shell_data();
	exit(EXIT_SUCCESS);
}

static void	read_input(char **input, const char *limiter)
{
	char	*temp;
	char	*line;

	line = NULL;
	debug("[read_input] called - pid : %d, limiter : %s", getpid(), limiter);
	while (TRUE)
	{
		restore_tty();
		debug("[read_input] while loop - pid : %d", getpid());
		line = readline("heredoc> ");
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
			break ;
		debug("[read_input] while loop - pid : %d, line : %s", getpid(), line);
		temp = *input;
		if (!temp)
		{
			*input = ft_strjoin(line, "\n");
			free(line);
			continue ;
		}
		*input = ft_multiplejoin(temp, line, "\n");
		debug("[read_input] while loop - pid : %d, input : %s", getpid(), *input);
		free(temp);
		free(line);
	}
	debug("[read_input] end - %s", *input);
}

static char	*read_from_pipe(int pipe_fd)
{
	char	*input;
	char	*read_;
	char	*temp;
	int		read_byte;
	char	buf[256];

	read_byte = read(pipe_fd, buf, 256);
	input = NULL;
	while (read_byte > 0)
	{
		read_ = ft_substr(buf, 0, read_byte);
		temp = input;
		input = ft_strjoin(temp, read_);
		if (temp)
			free(temp);
		free(read_);
		read_byte = read(pipe_fd, buf, 256);
	}
	return (input);
}

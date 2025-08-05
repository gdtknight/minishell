/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:39:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:01:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "def.h"
#include "libft.h"

#include "eval.h"
#include "parser.h"
#include "utils.h"

static t_status	set_stdin(t_syntax_node *io_redir_node);
static t_status	set_stdout(t_syntax_node *io_redir_node);

int	eval_io_redir(t_syntax_node *io_redir_node)
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
	int	infile_fd;

	if (io_redir_node->type == NODE_IO_REDIR_IN)
	{
		infile_fd = open(io_redir_node->value.io_target, O_RDONLY);
		if (infile_fd == -1)
		{
			perror(strerror(errno));
			return (1);
		}
		close(STDIN_FILENO);
		dup2(infile_fd, STDIN_FILENO);
	}
	else
		ft_putendl_fd(heredoc(io_redir_node->value.io_target), STDIN_FILENO);
	return (0);
}

static t_status	set_stdout(t_syntax_node *io_redir_node)
{
	int	outfile_fd;

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
		return (1);
	}
	close(STDOUT_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	return (0);
}

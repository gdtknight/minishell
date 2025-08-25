/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 03:30:48 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 14:23:53 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>

#include "ast.h"
#include "shell.h"
#include "eval.h"

void	wait_child(pid_t child_pid, int *status, int options)
{
	waitpid(child_pid, status, options);
	if (WIFEXITED(*status))
		(get_shell_data())->last_status = WEXITSTATUS(*status);
	if (WIFSIGNALED(*status))
		(get_shell_data())->last_status = 128 + WTERMSIG(*status);
}

t_syntax_node	*find_cmd_node(t_syntax_node *io_redir_node)
{
	t_syntax_node	*cur;

	cur = io_redir_node;
	while (cur->type != NODE_SIMPLE_COMMAND)
	{
		if (cur->type == NODE_SEMICOLON
			|| cur->type == NODE_AMPERSAND
			|| cur->type == NODE_AND_IF
			|| cur->type == NODE_OR_IF
			|| cur->type == NODE_PIPELINE
			|| cur->type == NODE_PIPELINE_ERR)
			return (NULL);
		cur = cur->parent;
	}
	return (cur);
}

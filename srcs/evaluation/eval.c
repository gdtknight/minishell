/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:35:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 15:47:10 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "eval.h"
#include "parser.h"
#include "debug.h"

int	eval(t_syntax_node *node)
{
	int	status;

	debug("eval called ");
	status = 0;
	if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND)
		status = eval_list(node);
	else if (node->type == NODE_AND_IF || node->type == NODE_OR_IF)
		status = eval_and_or(node);
	else if (node->type == NODE_PIPELINE || node->type == NODE_PIPELINE_ERR)
		status = eval_pipeline(node);
	else if (node->type == NODE_SIMPLE_COMMAND
		|| node->type == NODE_COMPOUND_COMMAND)
		status = eval_command(node);
	return (status);
}

/*
 * 현재는 & 기호 지원하지 않음.
 * NODE_SEMICOLON, NODE_AMPERSAND 모두 동기적으로 처리
 */
int	eval_list(t_syntax_node *list_node)
{
	int		status;
	int		child_pid;

	child_pid = fork();
	if (child_pid == 0)
		exit(eval(list_node->value.b_node.left));
	else
	{
		waitpid(child_pid, &status, 0);
		eval(list_node->value.b_node.right);
	}
	return (status);
}

int	eval_and_or(t_syntax_node *and_or_node)
{
	int	status;

	status = eval(and_or_node->value.b_node.left);
	if (and_or_node->type == NODE_AND_IF && status == 0)
		status = eval(and_or_node->value.b_node.right);
	else if (and_or_node->type == NODE_OR_IF && status != 0)
		status = eval(and_or_node->value.b_node.right);
	return (status);
}

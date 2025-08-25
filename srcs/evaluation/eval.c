/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:35:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:09:01 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

#include "def.h"
#include "ast.h"
#include "shell.h"

#include "eval.h"

/**
 * @brief Evaluates a syntax node and dispatches to the correct handler.
 *
 * Calls the appropriate evaluation function based on the node type.
 *
 * @param node Pointer to the syntax node to evaluate.
 */
void	eval(t_syntax_node *node)
{
	if (!node || node->eval == OFF)
		return ;
	if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND)
	{
		eval_list(node);
		return ;
	}
	if (node->type == NODE_AND_IF || node->type == NODE_OR_IF)
	{
		eval_and_or(node);
		return ;
	}
	if (node->type == NODE_PIPELINE || node->type == NODE_PIPELINE_ERR)
	{
		eval_pipeline(node);
		return ;
	}
	if (node->type == NODE_SIMPLE_COMMAND
		|| node->type == NODE_COMPOUND_COMMAND)
	{
		eval_command(node);
		return ;
	}
	(get_shell_data())->last_status = EXIT_FAILURE;
}

/**
 * @brief Evaluates a list node (semicolon or ampersand).
 *
 * Handles sequential and background execution of left and right nodes.
 *
 * @param node Pointer to the list node to evaluate.
 */
void	eval_list(t_syntax_node *node)
{
	int			status;
	pid_t		child_pid;

	if (!node || node->eval == OFF)
		return ;
	if (node->type == NODE_SEMICOLON)
	{
		eval(node->value.b_node.left);
		eval(node->value.b_node.right);
		return ;
	}
	child_pid = fork();
	if (child_pid == 0)
	{
		eval(node->value.b_node.left);
		return ;
	}
	waitpid(child_pid, &status, WNOHANG);
	get_shell_data()->last_bg_pid = child_pid;
	eval(node->value.b_node.right);
}

/**
 * @brief Evaluates an AND/OR node.
 *
 * Evaluates the left node, then the right node based on the last status and
 * the node type (AND_IF or OR_IF).
 *
 * @param and_or_node Pointer to the AND/OR node to evaluate.
 */
void	eval_and_or(t_syntax_node *and_or_node)
{
	if (!and_or_node || and_or_node->eval == OFF)
		return ;
	eval(and_or_node->value.b_node.left);
	if (and_or_node->type == NODE_AND_IF
		&& (get_shell_data())->last_status == SUCCESS)
		eval(and_or_node->value.b_node.right);
	else if (and_or_node->type == NODE_OR_IF
		&& (get_shell_data())->last_status != SUCCESS)
		eval(and_or_node->value.b_node.right);
}

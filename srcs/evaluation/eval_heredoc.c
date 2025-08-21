/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:57:31 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 08:42:37 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"

static void	eval_heredoc_construct(t_syntax_node *node);
static void	eval_heredoc_command(t_syntax_node *node);

void	eval_heredoc(t_syntax_node *node)
{
	if (!node || node->eval == OFF)
		return ;
	if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND
		|| node->type == NODE_AND_IF || node->type == NODE_OR_IF
		|| node->type == NODE_PIPELINE || node->type == NODE_PIPELINE_ERR)
		eval_heredoc_construct(node);
	else if (node->type == NODE_COMPOUND_COMMAND
		|| node->type == NODE_SIMPLE_COMMAND
		|| node->type == NODE_CMD_PREFIX || node->type == NODE_CMD_PREFIX)
		eval_heredoc_command(node);
	else if (node->type == NODE_IO_REDIR_HEREDOC)
		read_heredoc(node);
}

static void	eval_heredoc_construct(t_syntax_node *node)
{
	if (!node)
		return ;
	if (node->value.b_node.left)
		eval_heredoc(node->value.b_node.left);
	if (node->value.b_node.right)
		eval_heredoc(node->value.b_node.right);
}

static void	eval_heredoc_command(t_syntax_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMPOUND_COMMAND && node->value.child)
		eval_heredoc(node->value.child);
	else if (node->type == NODE_SIMPLE_COMMAND)
	{
		if (node->value.command.prefix)
			eval_heredoc(node->value.command.prefix);
		if (node->value.command.suffix)
			eval_heredoc(node->value.command.suffix);
	}
	else if (node->type == NODE_CMD_PREFIX || node->type == NODE_CMD_PREFIX)
	{
		if (node->value.b_node.left)
			eval_heredoc(node->value.b_node.left);
		if (node->value.b_node.right)
			eval_heredoc(node->value.b_node.right);
	}
}

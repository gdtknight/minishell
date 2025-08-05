/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:23:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 19:02:43 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "parser.h"

static void	print_cmd_node(t_syntax_node *node, int depth);
static void	print_binary_node_type(t_syntax_node *node, int depth);
static void	print_leaf_node(t_syntax_node *node, int depth);

void	print_node(t_syntax_node *node, int depth)
{
	if (!node)
		return ;
	else if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND
		|| node->type == NODE_AND_IF || node->type == NODE_OR_IF
		|| node->type == NODE_PIPELINE
		|| node->type == NODE_PIPELINE_ERR)
		print_binary_node_type(node, depth);
	else if (node->type == NODE_SIMPLE_COMMAND
		|| node->type == NODE_CMD_PREFIX
		|| node->type == NODE_CMD_SUFFIX)
		print_cmd_node(node, depth);
	else
		print_leaf_node(node, depth);
}

static void	print_binary_node_type(t_syntax_node *node, int depth)
{
	if (node->type == NODE_SEMICOLON)
		printf("[%02d] type - %s\n", depth, "node_semicolon (;)");
	if (node->type == NODE_AMPERSAND)
		printf("[%02d] type - %s\n", depth, "node_ampersand (&)");
	if (node->type == NODE_AND_IF)
		printf("[%02d] type - %s\n", depth, "node_and_if (&&)");
	if (node->type == NODE_OR_IF)
		printf("[%02d] type - %s\n", depth, "node_or_if (||)");
	if (node->type == NODE_PIPELINE)
		printf("[%02d] type - %s\n", depth, "node_pipeline (|)");
	if (node->type == NODE_PIPELINE_ERR)
		printf("[%02d] type - %s\n", depth, "node_pipeline_err (|&)");
	print_node(node->value.b_node.left, depth + 1);
	print_node(node->value.b_node.right, depth + 1);
}

static void	print_cmd_node(t_syntax_node *node, int depth)
{
	if (node->type == NODE_SIMPLE_COMMAND)
	{
		printf("[%02d] type - %s\n", depth, "simple_command");
		print_node(node->value.command.prefix, depth);
		printf("cmd - %s\n", node->value.command.word);
		print_node(node->value.command.suffix, depth);
	}
	else if (node->type == NODE_CMD_PREFIX)
	{
		printf("[%02d] type - %s\n", depth, "cmd_prefix");
		print_node(node->value.b_node.left, depth + 1);
		print_node(node->value.b_node.right, depth + 1);
	}
	else if (node->type == NODE_CMD_SUFFIX)
	{
		printf("[%02d] type - %s\n", depth, "cmd_suffix");
		print_node(node->value.b_node.left, depth + 1);
		print_node(node->value.b_node.right, depth + 1);
	}
}

static void	print_leaf_node(t_syntax_node *node, int depth)
{
	if (node->type == NODE_IO_REDIR_OUT
		|| node->type == NODE_IO_REDIR_APPEND
		|| node->type == NODE_IO_REDIR_IN
		|| node->type == NODE_IO_REDIR_HEREDOC)
		printf("[%02d] type - io_redir, target - %s\n", \
			depth, node->value.io_target);
	if (node->type == NODE_ASSIGN_WORD)
		printf("[%02d] type - %s\n", depth, "assign_word");
	if (node->type == NODE_WORD)
		printf("[%02d] type - word, value - %s\n", depth, node->value.word);
}

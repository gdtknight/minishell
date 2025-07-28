/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:45:14 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 02:29:54 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "syntax_tree.h"

t_syntax_node	*create_empty_node(void)
{
	t_syntax_node	*node;

	node = (t_syntax_node *) malloc(sizeof(t_syntax_node));
	if (!node)
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	return (node);
}

void	remove_syntax_node(t_syntax_node *target)
{
	if (!target)
		return ;
	if (target->type == NODE_WORD
		|| target->type == NODE_ASSIGN_WORD)
	{
		free(target->value.word);
	}
	if (target->type == NODE_IO_REDIR_IN
		|| target->type == NODE_IO_REDIR_OUT
		|| target->type == NODE_IO_REDIR_HEREDOC
		|| target->type == NODE_IO_REDIR_APPEND)
	{
		free(target->value.u_operator.operator);
		remove_syntax_node(target->value.u_operator.target);
		free(target);
	}
	if (target->type == NODE_CMD_WORD)
		remove_syntax_node(target->value.child);
	if (target->type == NODE_CMD_PREFIX
		|| target->type == NODE_CMD_SUFFIX)
	{
		remove_syntax_node(target->value.b_node.left);
		remove_syntax_node(target->value.b_node.right);
	}
	if (target->type == NODE_SIMPLE_COMMAND)
	{
		remove_syntax_node(target->value.command.prefix);
		remove_syntax_node(target->value.command.word);
		remove_syntax_node(target->value.command.suffix);
	}
	if (target->type == NODE_LIST
		|| target->type == NODE_AND_OR
		|| target->type == NODE_PIPELINE
		|| target->type == NODE_PIPELINE_ERR
	)
	{
		free(target->value.b_operator.operator);
		remove_syntax_node(target->value.b_operator.left);
		remove_syntax_node(target->value.b_operator.right);
	}
	free(target);
}

void	print_node_type(t_syntax_node *node)
{
	if (node->type == NODE_LIST
		|| node->type == NODE_AND_OR
		|| node->type == NODE_PIPELINE
		|| node->type == NODE_PIPELINE_ERR)
		printf("type - %s\n", node->value.b_operator.operator);
	if (node->type == NODE_SIMPLE_COMMAND)
		printf("type - %s\n", "simple_command");
	if (node->type == NODE_CMD_PREFIX)
		printf("type - %s\n", "cmd_prefix");
	if (node->type == NODE_CMD_WORD)
		printf("type - %s\n", "cmd_word");
	if (node->type == NODE_CMD_SUFFIX)
		printf("type - %s\n", "cmd_suffix");
	if (node->type == NODE_ASSIGN_WORD)
		printf("type - %s\n", "assign_word");
	if (node->type == NODE_IO_REDIR_OUT)
		printf("type - %s\n", node->value.u_operator.operator);
	if (node->type == NODE_IO_REDIR_APPEND)
		printf("type - %s\n", node->value.u_operator.operator);
	if (node->type == NODE_IO_REDIR_IN)
		printf("type - %s\n", node->value.u_operator.operator);
	if (node->type == NODE_IO_REDIR_HEREDOC)
		printf("type - %s\n", node->value.u_operator.operator);
	if (node->type == NODE_WORD)
		printf("type - %s\n", "word");
}

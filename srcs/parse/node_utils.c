/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:45:14 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:23:37 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "parser.h"

static void	remove_string_value(t_syntax_node *node);

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

void	remove_syntax_node(t_syntax_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_WORD || node->type == NODE_ASSIGN_WORD
		|| node->type == NODE_IO_REDIR_IN || node->type == NODE_IO_REDIR_OUT
		|| node->type == NODE_IO_REDIR_APPEND)
		remove_string_value(node);
	else if (node->type == NODE_COMPOUND_COMMAND)
		remove_syntax_node(node->value.child);
	else if (node->type == NODE_SIMPLE_COMMAND)
	{
		remove_syntax_node(node->value.command.prefix);
		free(node->value.command.word);
		remove_syntax_node(node->value.command.suffix);
	}
	else if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND
		|| node->type == NODE_AND_IF || node->type == NODE_OR_IF
		|| node->type == NODE_PIPELINE
		|| node->type == NODE_PIPELINE_ERR
		|| node->type == NODE_CMD_PREFIX || node->type == NODE_CMD_SUFFIX)
	{
		remove_syntax_node(node->value.b_node.left);
		remove_syntax_node(node->value.b_node.right);
	}
	free(node);
}

static void	remove_string_value(t_syntax_node *node)
{
	if (node->type == NODE_WORD)
		free(node->value.word);
	else if (node->type == NODE_ASSIGN_WORD)
		free(node->value.assign_word);
	else if (node->type == NODE_IO_REDIR_IN
		|| node->type == NODE_IO_REDIR_OUT
		|| node->type == NODE_IO_REDIR_APPEND)
		free(node->value.io_target);
}

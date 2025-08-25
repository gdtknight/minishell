/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:45:14 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:11:41 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ast.h"

static void	initialize_value(t_syntax_node *node);

/**
 * @brief Creates and initializes an empty syntax node.
 *
 * Allocates memory for a new syntax node, sets default values, and
 * initializes its fields. Exits on allocation failure.
 *
 * @return t_syntax_node* Pointer to the newly created node.
 */
t_syntax_node	*create_empty_node(void)
{
	t_syntax_node	*node;

	node = (t_syntax_node *) malloc(sizeof(t_syntax_node));
	if (!node)
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	node->eval = ON;
	node->parent = NULL;
	initialize_value(node);
	return (node);
}

/**
 * @brief Initializes the value fields of a syntax node to NULL.
 *
 * Sets all union members in the node's value to NULL to ensure a clean
 * state.
 *
 * @param node Pointer to the syntax node to initialize.
 */
static void	initialize_value(t_syntax_node *node)
{
	node->value.assign_word = NULL;
	node->value.word = NULL;
	node->value.io_target = NULL;
	node->value.child = NULL;
}

/**
 * @brief Recursively disables evaluation for a syntax node and its children.
 *
 * Sets the eval flag to OFF for the node and all relevant child nodes,
 * depending on the node type.
 *
 * @param node Pointer to the syntax node to update.
 */
void	turnoff_node_eval(t_syntax_node *node)
{
	if (!node)
		return ;
	node->eval = OFF;
	if (node->type == NODE_COMPOUND_COMMAND)
		turnoff_node_eval(node->value.child);
	else if (node->type == NODE_SIMPLE_COMMAND)
	{
		turnoff_node_eval(node->value.command.prefix);
		turnoff_node_eval(node->value.command.suffix);
	}
	else if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND
		|| node->type == NODE_AND_IF || node->type == NODE_OR_IF
		|| node->type == NODE_PIPELINE
		|| node->type == NODE_PIPELINE_ERR
		|| node->type == NODE_CMD_PREFIX || node->type == NODE_CMD_SUFFIX)
	{
		turnoff_node_eval(node->value.b_node.left);
		turnoff_node_eval(node->value.b_node.right);
	}
}

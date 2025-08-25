/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:11:42 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 01:27:53 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ast.h"
#include "eval.h"

static void	remove_leaf_node(t_syntax_node *node);
static void	remove_command_node(t_syntax_node *node);
static void	remove_binary_node(t_syntax_node *node);

/**
 * @brief Recursively frees a syntax node and its children.
 *
 * Determines the node type and calls the appropriate removal function.
 *
 * @param node Pointer to the syntax node to remove.
 */
void	remove_syntax_node(t_syntax_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_WORD || node->type == NODE_ASSIGN_WORD
		|| node->type == NODE_IO_REDIR_IN || node->type == NODE_IO_REDIR_OUT
		|| node->type == NODE_IO_REDIR_APPEND
		|| node->type == NODE_IO_REDIR_HEREDOC)
	{
		remove_leaf_node(node);
		return ;
	}
	if ((node->type == NODE_COMPOUND_COMMAND)
		|| (node->type == NODE_SIMPLE_COMMAND))
	{
		remove_command_node(node);
		return ;
	}
	remove_binary_node(node);
}

/**
 * @brief Frees a leaf node and its associated memory.
 *
 * Handles freeing for word, assign_word, and I/O redirection nodes.
 *
 * @param node Pointer to the leaf node to remove.
 */
static void	remove_leaf_node(t_syntax_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_WORD)
	{
		free(node->value.word);
		node->value.word = NULL;
	}
	else if (node->type == NODE_ASSIGN_WORD)
	{
		free(node->value.assign_word);
		node->value.assign_word = NULL;
	}
	else if (node->type == NODE_IO_REDIR_IN
		|| node->type == NODE_IO_REDIR_OUT
		|| node->type == NODE_IO_REDIR_APPEND
		|| node->type == NODE_IO_REDIR_HEREDOC)
	{
		free(node->value.io_target);
		node->value.io_target = NULL;
	}
	free(node);
}

/**
 * @brief Frees a command node and its children.
 *
 * Handles both compound and simple command nodes, freeing all associated
 * memory.
 *
 * @param node Pointer to the command node to remove.
 */
static void	remove_command_node(t_syntax_node *node)
{
	if (node->type == NODE_COMPOUND_COMMAND)
	{
		remove_syntax_node(node->value.child);
		node->value.child = NULL;
		free(node);
		return ;
	}
	remove_syntax_node(node->value.command.prefix);
	remove_syntax_node(node->value.command.suffix);
	if (node->value.command.cmd_word)
		remove_syntax_node(node->value.command.cmd_word);
	node->value.command.cmd_word = NULL;
	node->value.command.prefix = NULL;
	node->value.command.suffix = NULL;
	clear_cmd_form(&(node->value.command.form));
	free(node);
	return ;
}

/**
 * @brief Frees a binary node and its left/right children.
 *
 * Handles nodes with left and right children, freeing all associated memory.
 *
 * @param node Pointer to the binary node to remove.
 */
static void	remove_binary_node(t_syntax_node *node)
{
	remove_syntax_node(node->value.b_node.left);
	remove_syntax_node(node->value.b_node.right);
	node->value.b_node.left = NULL;
	node->value.b_node.right = NULL;
	free(node);
	return ;
}

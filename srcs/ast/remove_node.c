/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:11:42 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 22:11:42 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ast.h"
#include "eval.h"

static void	remove_leaf_node(t_syntax_node *node);
static void	remove_command_node(t_syntax_node *node);
static void	remove_binary_node(t_syntax_node *node);

/**
 * @brief 구문 트리 노드를 해제한다.
 *
 * 노드 타입에 따라 value에 포함된 동적 메모리를 먼저 해제하고,
 * 재귀적으로 하위 노드까지 해제한 뒤 현재 노드를 free 한다.
 *
 * @param node 해제할 노드 포인터
 *
 * @note
 * - NODE_WORD, NODE_ASSIGN_WORD, NODE_IO_REDIR_* 타입은 문자열 값만 해제한다.
 * - NODE_COMPOUND_COMMAND는 하위 child 노드를 재귀적으로 해제한다.
 * - NODE_SIMPLE_COMMAND는 prefix, suffix, word를 해제한다.
 * - 이진 구조(NODE_SEMICOLON, NODE_AND_IF, NODE_PIPELINE 등)는 left/right 모두 재귀 해제한다.
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
 * @brief 문자열 값을 가진 노드의 value를 해제한다.
 *
 * NODE_WORD, NODE_ASSIGN_WORD, NODE_IO_REDIR_* 타입에서
 * 해당하는 문자열 포인터를 free 한다.
 *
 * @param node 문자열 값을 가진 노드 포인터
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
	if (node->value.command.word)
		free(node->value.command.word);
	node->value.command.word = NULL;
	node->value.command.prefix = NULL;
	node->value.command.suffix = NULL;
	clear_cmd_form(&(node->value.command.form));
	free(node);
	return ;
}

static void	remove_binary_node(t_syntax_node *node)
{
	remove_syntax_node(node->value.b_node.left);
	remove_syntax_node(node->value.b_node.right);
	node->value.b_node.left = NULL;
	node->value.b_node.right = NULL;
	free(node);
	return ;
}

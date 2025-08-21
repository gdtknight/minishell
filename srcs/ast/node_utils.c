/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:45:14 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 12:11:38 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ast.h"
#include "eval.h"

static void	remove_leaf_node(t_syntax_node *node);
static void	initialize_value(t_syntax_node *node);

/**
 * @brief 비어 있는 구문 트리 노드를 생성한다.
 *
 * t_syntax_node 구조체를 동적 할당하여 반환한다.
 * 메모리 할당에 실패하면 에러를 출력하고 프로그램을 종료한다.
 *
 * @return t_syntax_node* 새로 생성된 노드 포인터
 *
 * @note
 * - 생성된 노드는 type이나 value가 초기화되지 않으므로, 호출자가 반드시 초기화해야 한다.
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
	node->eval	= ON;
	node->parent = NULL;
	initialize_value(node);
	return (node);
}

static void	initialize_value(t_syntax_node *node)
{
	node->value.assign_word = NULL;
	node->value.word = NULL;
	node->value.io_target = NULL;
	node->value.child = NULL;
}

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
		|| node->type == NODE_IO_REDIR_APPEND || node->type == NODE_IO_REDIR_HEREDOC)
	{
		remove_leaf_node(node);
		return ;
	}
	if (node->type == NODE_COMPOUND_COMMAND)
	{
		remove_syntax_node(node->value.child);
		node->value.child = NULL;
		free(node);
		return ;
	}
	if (node->type == NODE_SIMPLE_COMMAND)
	{
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
	if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND
		|| node->type == NODE_AND_IF || node->type == NODE_OR_IF
		|| node->type == NODE_PIPELINE
		|| node->type == NODE_PIPELINE_ERR
		|| node->type == NODE_CMD_PREFIX || node->type == NODE_CMD_SUFFIX)
	{
		remove_syntax_node(node->value.b_node.left);
		remove_syntax_node(node->value.b_node.right);
		node->value.b_node.left = NULL;
		node->value.b_node.right = NULL;
		free(node);
		return ;
	}
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

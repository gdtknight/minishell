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
	node->eval = ON;
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

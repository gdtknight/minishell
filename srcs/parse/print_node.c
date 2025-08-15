/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:23:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/14 02:06:57 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "parser.h"

static void	print_cmd_node(t_syntax_node *node, int depth);
static void	print_binary_node_type(t_syntax_node *node, int depth);
static void	print_leaf_node(t_syntax_node *node, int depth);

/**
 * @brief 구문 트리 노드를 깊이 정보와 함께 사람이 읽기 쉬운 형태로 출력한다.
 *
 * 이진 노드(;, &, &&, ||, |, |&)는 print_binary_node_type()로,
 * 명령 노드(NODE_SIMPLE_COMMAND, NODE_CMD_PREFIX, NODE_CMD_SUFFIX)는
 * print_cmd_node()로, 그 외 리프 노드는 print_leaf_node()로 위임한다.
 *
 * @param node  출력할 노드
 * @param depth 현재 출력 깊이(루트는 0)
 */
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

/**
 * @brief 이진 연산 노드(;, &, &&, ||, |, |&)의 타입을 출력하고 좌/우 자식을 재귀 출력한다.
 *
 * @param node  이진 노드(NODE_SEMICOLON, NODE_AMPERSAND, NODE_AND_IF,
 *              NODE_OR_IF, NODE_PIPELINE, NODE_PIPELINE_ERR)
 * @param depth 현재 출력 깊이
 */
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

/**
 * @brief 명령 계열 노드(NODE_SIMPLE_COMMAND, NODE_CMD_PREFIX, NODE_CMD_SUFFIX)를 포맷에 맞춰 출력한다.
 *
 * - NODE_SIMPLE_COMMAND: prefix → "cmd - <word>" → suffix 순으로 출력
 * - NODE_CMD_PREFIX / NODE_CMD_SUFFIX: 좌/우 자식을 재귀 출력
 *
 * @param node  명령 계열 노드
 * @param depth 현재 출력 깊이
 */
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

/**
 * @brief 리프 노드(리다이렉션, 단어, 할당)를 한 줄로 출력한다.
 *
 * - NODE_IO_REDIR_*: "io_redir, target - <io_target>"
 * - NODE_ASSIGN_WORD: "assign_word"
 * - NODE_WORD: "word, value - <word>"
 *
 * @param node  리프 노드
 * @param depth 현재 출력 깊이
 */
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

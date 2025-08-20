/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   construct_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:01:20 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 16:24:14 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "tokenizer.h"

#include "ast.h"

/**
 * @brief <list> 구문 규칙에 따라 구문 트리를 구성한다.
 *
 * <list> ::= <and_or>
 *          | <list> ";" <and_or>
 *          | <list> "&" <and_or>
 *
 * @param tk_lst 현재 토큰 리스트 포인터의 주소.  
 *               함수 실행 중 토큰 리스트 포인터가 진행된다.
 * @return 구문 트리의 루트 노드 포인터.  
 *         메모리 할당 실패 시 NULL을 반환할 수 있다.
 */
t_syntax_node	*list(t_token **tk_lst)
{
	t_syntax_node	*list_node;
	t_syntax_node	*temp;

	list_node = and_or(tk_lst);
	temp = (NULL);
	while ((*tk_lst)->type == TK_SEMICOLON || (*tk_lst)->type == TK_AMPERSAND)
	{
		temp = list_node;
		list_node = create_empty_node();
		if ((*tk_lst)->type == TK_SEMICOLON)
			list_node->type = NODE_SEMICOLON;
		else
			list_node->type = NODE_AMPERSAND;
		list_node->value.b_node.left = temp;
		(*tk_lst) = (*tk_lst)->next;
		list_node->value.b_node.right = and_or(tk_lst);
		list_node->value.b_node.left->parent = list_node;
		if (list_node->value.b_node.right)
			list_node->value.b_node.right->parent = list_node;
	}
	return (list_node);
}

/**
 * @brief <and_or> 구문 규칙에 따라 구문 트리를 구성한다.
 *
 * <and_or> ::= <pipeline>
 *            | <and_or> "&&" <pipeline>
 *            | <and_or> "||" <pipeline>
 *
 * @param tk_lst 현재 토큰 리스트 포인터의 주소.  
 *               함수 실행 중 토큰 리스트 포인터가 진행된다.
 * @return 구문 트리의 루트 노드 포인터.  
 *         메모리 할당 실패 시 NULL을 반환할 수 있다.
 */
t_syntax_node	*and_or(t_token **tk_lst)
{
	t_syntax_node	*and_or_node;
	t_syntax_node	*temp;

	and_or_node = pipeline(tk_lst);
	temp = (NULL);
	while ((*tk_lst)->type == TK_AND_IF || (*tk_lst)->type == TK_OR_IF)
	{
		temp = and_or_node;
		and_or_node = create_empty_node();
		if ((*tk_lst)->type == TK_AND_IF)
			and_or_node->type = NODE_AND_IF;
		else
			and_or_node->type = NODE_OR_IF;
		and_or_node->value.b_node.left = temp;
		(*tk_lst) = (*tk_lst)->next;
		and_or_node->value.b_node.right = pipeline(tk_lst);
		and_or_node->value.b_node.left->parent = and_or_node;
		if (and_or_node->value.b_node.right)
			and_or_node->value.b_node.right->parent = and_or_node;
	}
	return (and_or_node);
}

/**
 * @brief <pipeline> 구문 규칙에 따라 구문 트리를 구성한다.
 *
 * <pipeline> ::= <command>
 *              | <pipeline> "|" <command>
 *              | <pipeline> "|&" <command>
 *
 * @param tk_lst 현재 토큰 리스트 포인터의 주소.  
 *               함수 실행 중 토큰 리스트 포인터가 진행된다.
 * @return 구문 트리의 루트 노드 포인터.  
 *         메모리 할당 실패 시 NULL을 반환할 수 있다.
 */
t_syntax_node	*pipeline(t_token **tk_lst)
{
	t_syntax_node	*pipeline_node;
	t_syntax_node	*temp;

	pipeline_node = command(tk_lst);
	temp = (NULL);
	while ((*tk_lst)->type == TK_PIPE || (*tk_lst)->type == TK_PIPE_ERR)
	{
		temp = pipeline_node;
		pipeline_node = create_empty_node();
		if ((*tk_lst)->type == TK_PIPE)
			pipeline_node->type = NODE_PIPELINE;
		else
			pipeline_node->type = NODE_PIPELINE_ERR;
		pipeline_node->value.b_node.left = temp;
		pipeline_node->value.b_node.left->parent = pipeline_node;
		(*tk_lst) = (*tk_lst)->next;
		pipeline_node->value.b_node.right = command(tk_lst);
		if (pipeline_node->value.b_node.right)
			pipeline_node->value.b_node.right->parent = pipeline_node;
	}
	return (pipeline_node);
}

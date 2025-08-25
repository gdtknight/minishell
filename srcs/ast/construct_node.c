/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   construct_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:01:20 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:11:39 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "tokenizer.h"

#include "ast.h"

/**
 * @brief Parses a list of commands separated by semicolons or ampersands.
 *
 * Builds a binary tree of command lists, handling ';' and '&' operators.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The root node of the list, or NULL on failure.
 */
t_syntax_node	*list(t_token **tk_lst)
{
	t_syntax_node	*list_node;
	t_syntax_node	*temp;

	list_node = and_or(tk_lst);
	temp = (NULL);
	while ((*tk_lst) && \
		((*tk_lst)->type == TK_SEMICOLON || (*tk_lst)->type == TK_AMPERSAND))
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
 * @brief Parses a sequence of commands connected by '&&' or '||'.
 *
 * Builds a binary tree for logical AND/OR operations between pipelines.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The root node of the and/or sequence, or NULL
 * on failure.
 */
t_syntax_node	*and_or(t_token **tk_lst)
{
	t_syntax_node	*and_or_node;
	t_syntax_node	*temp;

	and_or_node = pipeline(tk_lst);
	temp = (NULL);
	while (*tk_lst && \
		((*tk_lst)->type == TK_AND_IF || (*tk_lst)->type == TK_OR_IF))
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
 * @brief Parses a pipeline of commands separated by '|' or '|&'.
 *
 * Builds a binary tree for pipelines, handling both standard and error
 * pipelines.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The root node of the pipeline, or NULL on failure.
 */
t_syntax_node	*pipeline(t_token **tk_lst)
{
	t_syntax_node	*pipeline_node;
	t_syntax_node	*temp;

	pipeline_node = command(tk_lst);
	temp = (NULL);
	while ((*tk_lst) && \
		((*tk_lst)->type == TK_PIPE || (*tk_lst)->type == TK_PIPE_ERR))
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   construct_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:01:20 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/29 20:09:34 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "tokenizer.h"
#include "parser.h"

/*
 * <list> ::= <and_or>
 *          | <list> ";" <and_or>
 *          | <list> "&" <and_or>
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
	}
	return (list_node);
}

/*
 * <and_or> ::= <pipeline>
 *            | <and_or> "&&" <pipeline>
 *            | <and_or> "||" <pipeline>
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
	}
	return (and_or_node);
}

/*
 * <pipeline> ::= <command>
 *              | <pipeline> "|" <command>
 *              | <pipeline> "|&" <command>
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
		(*tk_lst) = (*tk_lst)->next;
		pipeline_node->value.b_node.right = command(tk_lst);
	}
	return (pipeline_node);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:29:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/24 20:37:19 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "debug.h"
#include "syntax_tree.h"

/*
 * <shell_input>  ::= <list> <newline_list>
 * <newline_list> ::= <newline>
 *                  | <newline_list> <newline>
 * <newline>      ::= "\n"
 */
t_syntax_node	*shell_input(t_token **tk_lst)
{
	t_syntax_node	*input_node;

	debug("shell_input called");
	input_node = list(tk_lst);
	while ((*tk_lst)->type == TK_NEWLINE)
		(*tk_lst) = (*tk_lst)->next;
	if((*tk_lst)->type != TK_EOF)
	{
		printf("invalid input");
		return (NULL);
	}
	debug("shell_input success");
	print_node_type(input_node);
	return (input_node);
}

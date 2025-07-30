/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:29:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/29 20:10:07 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "parser.h"

/*
 * <shell_input>  ::= <list> <newline_list>
 * <newline_list> ::= <newline>
 *                  | <newline_list> <newline>
 * <newline>      ::= "\n"
 */
t_syntax_node	*shell_input(t_token **tk_lst)
{
	t_syntax_node	*input_node;

	input_node = list(tk_lst);
	while ((*tk_lst)->type == TK_NEWLINE)
		(*tk_lst) = (*tk_lst)->next;
	if ((*tk_lst)->type != TK_EOF)
	{
		printf("invalid input");
		return (NULL);
	}
	return (input_node);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:29:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:11:40 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "ast.h"

/**
 * @brief Parses the input tokens and returns the root of the syntax tree.
 *
 * Consumes newlines and checks for invalid input after parsing. Returns NULL
 * if invalid tokens remain after parsing.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The root node of the parsed syntax tree, or NULL on
 * error.
 */
t_syntax_node	*parse_input(t_token **tk_lst)
{
	t_syntax_node	*input_node;

	input_node = list(tk_lst);
	while ((*tk_lst) && ((*tk_lst)->type == TK_NEWLINE))
		(*tk_lst) = (*tk_lst)->next;
	if ((*tk_lst) && (*tk_lst)->type != TK_EOF)
	{
		printf("invalid input");
		return (NULL);
	}
	return (input_node);
}

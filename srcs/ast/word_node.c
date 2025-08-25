/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:47:58 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:11:43 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "tokenizer.h"

#include "ast.h"

/**
 * @brief Parses an assignment word node from the token list.
 *
 * Checks if the current token is a word containing '=' and creates an
 * assignment node if so.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The assignment word node, or NULL on failure.
 */
t_syntax_node	*assignment_word(t_token **tk_lst)
{
	t_syntax_node	*assignment_word;

	if ((*tk_lst)->type != TK_WORD)
		return (NULL);
	if (ft_strchr((*tk_lst)->value, '=') == NULL)
		return (NULL);
	assignment_word = create_empty_node();
	assignment_word->type = NODE_ASSIGN_WORD;
	assignment_word->value.assign_word = ft_strdup((*tk_lst)->value);
	(*tk_lst) = (*tk_lst)->next;
	return (assignment_word);
}

/**
 * @brief Parses a word node from the token list.
 *
 * Checks if the current token is a word and creates a word node if valid.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The word node, or NULL on failure.
 */
t_syntax_node	*word(t_token **tk_lst)
{
	t_syntax_node	*word_node;

	word_node = create_empty_node();
	if ((*tk_lst) == NULL || ((*tk_lst)->type != TK_WORD))
	{
		free (word_node);
		return (NULL);
	}
	word_node->type = NODE_WORD;
	word_node->value.word = ft_strdup((*tk_lst)->value);
	(*tk_lst) = (*tk_lst)->next;
	return (word_node);
}

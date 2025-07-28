/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:47:58 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 14:50:30 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "debug.h"
#include "libft.h"

#include "syntax_tree.h"
#include "tokenizer.h"

t_syntax_node	*assignment_word(t_token **tk_lst)
{
	t_syntax_node	*assignment_word;

	debug("assignment_word called - %s", (char *)((*tk_lst)->value));
	if ((*tk_lst)->type != TK_WORD
		&& (*tk_lst)->type != TK_WORD_WITH_SQUOTE
		&& (*tk_lst)->type != TK_WORD_WITH_DQUOTE)
	{
		debug("assign_word - not word");
		return (NULL);
	}
	if (ft_strchr((*tk_lst)->value, '=') == NULL)
	{
		debug("assign_word - not word");
		return (NULL);
	}
	assignment_word = create_empty_node();
	assignment_word->type = NODE_ASSIGN_WORD;
	assignment_word->value.word = ft_strdup((*tk_lst)->value);
	(*tk_lst) = (*tk_lst)->next;
	return (assignment_word);
}

t_syntax_node	*word(t_token **tk_lst)
{
	t_syntax_node	*word_node;

	debug("word called - %s", (char *)((*tk_lst)->value));
	word_node = create_empty_node();
	if ((*tk_lst) == NULL
		|| ((*tk_lst)->type != TK_WORD
			&& (*tk_lst)->type != TK_WORD_WITH_SQUOTE
			&& (*tk_lst)->type != TK_WORD_WITH_DQUOTE))
	{
		debug("tk is not word");
		free (word_node);
		return (NULL);
	}
	word_node->type = NODE_WORD;
	word_node->value.word = ft_strdup((*tk_lst)->value);
	debug("word node created - %s", (char *)((*tk_lst)->value));
	(*tk_lst) = (*tk_lst)->next;
	debug("next word - %s", (char *)(*tk_lst)->value);
	return (word_node);
}

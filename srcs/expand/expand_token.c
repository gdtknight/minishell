/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:26:39 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:44:01 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "tokenizer.h"
#include "expand.h"

static t_token	*new_token_from_exp_token_list(
					t_token *token,
					t_list *expand_lst);
static void		replace_token(
					t_token *token,
					t_token *first,
					t_token *new_token);

void	expand_heredoc_target(t_token **token)
{
	t_exp_token	*exp_token;

	if (!token || !*token || (*token)->type != TK_WORD)
		return ;
	exp_token = create_exp_token((*token)->value);
	quote_removal(exp_token);
	free((*token)->value);
	(*token)->value = ft_strdup(exp_token->value);
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
}

t_token	*expand_token(t_token *token)
{
	t_list		*expand_lst;
	t_token		*new_token;
	t_exp_token	*exp_token;

	if (!token || token->type != TK_WORD)
		return (NULL);
	new_token = NULL;
	expand_lst = NULL;
	exp_token = create_exp_token(token->value);
	exp_token = expand_tilde(exp_token);
	exp_token = expand_param(exp_token);
	split_field(&expand_lst, exp_token);
	if (!expand_lst)
		return (NULL);
	ft_lstiter(expand_lst, quote_removal);
	new_token = new_token_from_exp_token_list(token, expand_lst);
	ft_lstclear(&expand_lst, remove_exp_token);
	return (new_token);
}

static t_token	*new_token_from_exp_token_list(
					t_token *token,
					t_list *expand_lst)
{
	t_exp_token	*cur;
	t_token		*first;
	t_token		*new_token;
	t_token		*prev;

	if (!token || !expand_lst)
		return (token);
	first = NULL;
	new_token = NULL;
	prev = NULL;
	while (expand_lst)
	{
		cur = (t_exp_token *) expand_lst->content;
		new_token = create_empty_token();
		new_token->type = TK_WORD;
		new_token->value = ft_strdup(cur->value);
		if (!first)
			first = new_token;
		if (prev)
		{
			prev->next = new_token;
			new_token->prev = prev;
		}
		prev = new_token;
		expand_lst = expand_lst->next;
	}
	replace_token(token, first, new_token);
	return (first);
}

static	void	replace_token(t_token *token, t_token *first, t_token *new_token)
{
	if (token->prev)
	{
		token->prev->next = first;
		first->prev = token->prev;
	}
	if (token->next)
	{
		new_token->next = token->next;
		token->next->prev = new_token;
	}
}

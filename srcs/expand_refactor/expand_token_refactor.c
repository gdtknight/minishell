/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_refactor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:26:39 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/24 22:44:20 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "debug.h"
#include "libft.h"

#include "tokenizer.h"
#include "expand_refactor.h"

static t_token	*new_token_from_exp_token_list(
					t_token *token,
					t_list *expand_lst);
static void		quote_removal(void *content);
static void		remove_exp_token(void *content);

t_token	*expand_token_refactor(t_token *token)
{
	t_list		*expand_lst;
	t_token		*new_token;
	t_exp_token	*exp_token;

	if (!token || token->type != TK_WORD)
		return (NULL);
	new_token = NULL;
	debug("[expand_token_refactor] called");
	debug("[expand_token_refactor] before tk_value : \'%s\'", (char *)(token->value));
	expand_lst = NULL;
	exp_token = create_exp_token(token->value);
	debug("[expand_token_refactor] exp_token_value : \'%s\'", (char *)(exp_token->value));
	exp_token = expand_tilde_refactor(exp_token);
	debug("[expand_token_refactor] expand_tilde_refactor completed");
	debug("[expand_token_refactor] exp_token_value : \'%s\'", (char *)(exp_token->value));
	exp_token = expand_param_refactor(exp_token);
	debug("[expand_token_refactor] expand_param_refactor completed");
	debug("[expand_token_refactor] exp_token_value : \'%s\'", (char *)(exp_token->value));
	split_field_refactor(&expand_lst, exp_token);
	debug("[expand_token_refactor] split_field_refactor completed");
	if (!expand_lst)
	{
		free(exp_token->value);
		free(exp_token->qmask);
		free(exp_token);
		return (NULL);
	}
	ft_lstiter(expand_lst, quote_removal);
	debug("[expand_token_refactor] ft_lstiter completed");
	new_token = new_token_from_exp_token_list(token, expand_lst);
	debug("[expand_token_refactor] new_token generated");
	ft_lstclear(&expand_lst, remove_exp_token);
	debug("[expand_token_refactor] clear expand_lst");
	if (!new_token)
		return (NULL);
	debug("[expand_token_refactor] after tk_value : \'%s\'", (char *)(new_token->value));
	debug("[expand_token_refactor] expand completed new_token address - %p", new_token);
	debug("[expand_token_refactor] expand completed new_token value address - %p", new_token->value);
	return (new_token);
}

void	expand_heredoc_target(t_token **token)
{
	t_exp_token	*exp_token;

	debug("[expand_heredoc_target] called");
	if (!token || !*token || (*token)->type != TK_WORD)
		return ;
	debug("[expand_heredoc_target] before tk_value : %s", (char *)((*token)->value));
	exp_token = create_exp_token((*token)->value);
	quote_removal(exp_token);
	free((*token)->value);
	(*token)->value = ft_strdup(exp_token->value);
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
	debug("[expand_heredoc_target] after tk_value : %s", (char *)((*token)->value));
}


/*
 *	TODO: 여기서 주소값이 제대로 갱신이 안되는 느낌인데 ..?
 */
static t_token	*new_token_from_exp_token_list(t_token *token, t_list *expand_lst)
{
	t_exp_token	*cur;
	t_token		*first;
	t_token	*new_token;
	t_token	*prev;

	if (!token || !expand_lst)
		return (token);
	debug("[new_token_from_exp_token_list] called");
	first = NULL;
	new_token = NULL;
	prev = NULL;
	while (expand_lst)
	{
		cur = (t_exp_token *) expand_lst->content;
		new_token = create_empty_token();
		new_token->type = TK_WORD;
		debug("[new_token_from_exp_token_list] cur->value : %s", cur->value);
		new_token->value = ft_strdup(cur->value);
		debug("[new_token_from_exp_token_list] new_token->value : %s (%p)", (char *)new_token->value, new_token->value);
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
	return (first);
}

static void	quote_removal(void *content)
{
	t_exp_token *exp_token;
	int	v_idx;
	int	m_idx;

	if (!content)
		return ;
	exp_token = (t_exp_token *)content;
	v_idx = 0;
	m_idx = 0;
	debug("[quote_removal] before exp_token->value : %s", (exp_token->value));
	while ((exp_token->qmask)[m_idx])
	{
		if ((exp_token->qmask)[m_idx] != MASK_QUOTED)
			(exp_token->value)[v_idx++] = (exp_token->value)[m_idx];
		m_idx++;
	}
	while (v_idx <= m_idx)
		(exp_token->value)[v_idx++] = '\0';
	debug("[quote_removal] after exp_token->value : %s", (exp_token->value));
}

static void	remove_exp_token(void *content)
{
	t_exp_token	*exp_token;

	exp_token = (t_exp_token *) content;
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
}

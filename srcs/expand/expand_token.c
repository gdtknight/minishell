/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:26:39 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 22:38:44 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expand_token.c
 * @brief Functions for expanding tokens in shell input.
 *
 * This module handles tilde expansion, parameter expansion, field splitting,
 * and quote removal for tokens. It produces a new token or token list
 * after expansion.
 */

#include <stdlib.h>

#include "libft.h"
#include "tokenizer.h"
#include "expand.h"

#include "debug.h"

/* Static helper declarations */
static t_token	*new_token_from_exp_token_list(
					t_token *token,
					t_list *expand_lst);
static void		replace_token(
					t_token *token,
					t_token *first_token,
					t_token *last_token);
static t_token	*extract_token(t_exp_token *exp_token);

/**
 * @brief Expand the target of a heredoc token.
 *
 * If the token is of type TK_WORD, perform quote removal on the token value.
 *
 * @param token Pointer to the token to expand
 */
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

/**
 * @brief Perform full expansion on a TK_WORD token.
 *
 * Steps:
 * 1. Create an expansion token from the original token value
 * 2. Apply tilde expansion
 * 3. Apply parameter expansion
 * 4. Split fields into a list of expansion tokens
 * 5. Remove quotes
 * 6. Convert the expansion token list into token list
 *
 * @param token Token to expand
 * @return t_token* New token list after expansion
 */
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
	debug("[expand_token] before value : %s", (char *)(token->value));
	debug("[expand_token] after value : %s", (char *)(new_token->value));
	return (new_token);
}

/**
 * @brief Convert a list of t_exp_token into a linked list of t_token.
 *
 * @param token Original token to replace
 * @param expand_lst List of expansion tokens
 * @return t_token* Head of the new token list
 */
static t_token	*new_token_from_exp_token_list(
					t_token *token,
					t_list *expand_lst)
{
	t_token		*new_token;
	t_token		*cur_token;
	t_token		*prev;

	if (!token || !expand_lst)
		return (token);
	new_token = NULL;
	cur_token = NULL;
	prev = NULL;
	while (expand_lst)
	{
		cur_token = extract_token(expand_lst->content);
		if (!new_token)
			new_token = cur_token;
		if (prev)
		{
			prev->next = cur_token;
			cur_token->prev = prev;
		}
		prev = cur_token;
		expand_lst = expand_lst->next;
	}
	replace_token(token, new_token, cur_token);
	return (new_token);
}

/**
 * @brief Replace an original token in the linked list with a new sequence.
 *
 * Adjusts previous and next pointers to maintain the linked list structure.
 *
 * @param token Original token to replace
 * @param first_token First token of the new sequence
 * @param last_token Last token of the new sequence
 */
static void	replace_token(
				t_token *token,
				t_token *first_token,
				t_token *last_token)
{
	if (token->prev)
	{
		token->prev->next = first_token;
		first_token->prev = token->prev;
	}
	if (token->next)
	{
		last_token->next = token->next;
		token->next->prev = last_token;
	}
}

/**
 * @brief Extract a new token from a t_exp_token.
 *
 * Allocates a new t_token of type TK_WORD and copies the value from
 * the expansion token.
 *
 * @param exp_token Source expansion token
 * @return t_token* Newly allocated token
 */
static t_token	*extract_token(t_exp_token *exp_token)
{
	t_token	*new_token;

	new_token = create_empty_token();
	new_token->type = TK_WORD;
	new_token->value = ft_strdup(exp_token->value);
	return (new_token);
}

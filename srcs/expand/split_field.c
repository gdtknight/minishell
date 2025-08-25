/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_field.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:56:12 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 16:33:54 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "def.h"
#include "utils.h"
#include "expand.h"

static void		update_state(t_boolean state[3], char c);

static void		add_token(
					t_list **exp_list,
					t_exp_token *exp_token,
					size_t *start,
					size_t *end);

static size_t	skip_ifs(const char *str, size_t idx);

void	split_field(t_list **exp_list, t_exp_token *exp_token)
{
	t_boolean	state[3];
	size_t		end;
	size_t		start;

	if (!exp_list || !exp_token)
		return ;
	state[IN_SQUOTE] = FALSE;
	state[IN_DQUOTE] = FALSE;
	state[IN_ESCAPE] = FALSE;
	end = skip_ifs(exp_token->value, 0);
	start = end;
	while (exp_token->value[end])
	{
		update_state(state, exp_token->value[end]);
		if (!state[IN_ESCAPE] && !state[IN_SQUOTE] && !state[IN_DQUOTE]
			&& is_ifs(exp_token->value[end]))
			add_token(exp_list, exp_token, &start, &end);
		else
			end++;
	}
	if (start != end)
		add_token(exp_list, exp_token, &start, &end);
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
}

static void	update_state(t_boolean state[3], char c)
{
	if (state[IN_ESCAPE])
		state[IN_ESCAPE] = FALSE;
	else if (c == '\\' && !state[IN_SQUOTE])
		state[IN_ESCAPE] = TRUE;
	else if (c == '\'' && !state[IN_DQUOTE])
		state[IN_SQUOTE] = !state[IN_SQUOTE];
	else if (c == '"' && !state[IN_SQUOTE])
		state[IN_DQUOTE] = !state[IN_DQUOTE];
}

static void	add_token(
				t_list **exp_list,
				t_exp_token *exp_token,
				size_t *start,
				size_t *end)
{
	t_exp_token	*new;

	new = create_empty_exp_token();
	new->value = ft_substr(&(exp_token->value)[*start], 0, *end - *start);
	new->qmask = ft_substr(&(exp_token->qmask)[*start], 0, *end - *start);
	ft_lstadd_back(exp_list, ft_lstnew(new));
	*end = skip_ifs(exp_token->value, *end);
	*start = *end;
}

static size_t	skip_ifs(const char *str, size_t idx)
{
	while (is_ifs(str[idx]))
		idx++;
	return (idx);
}

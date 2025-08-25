/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_field.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:56:12 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:39:13 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "def.h"
#include "utils.h"
#include "expand.h"

void	split_field(t_list **exp_list, t_exp_token *exp_token)
{
	t_exp_token	*new_exp_token;
	size_t			idx;
	size_t			last_idx;
	t_boolean	state[3];

	if (!exp_list || !exp_token)
		return ;
	state[IN_SQUOTE] = FALSE;
	state[IN_DQUOTE] = FALSE;
	state[IN_ESCAPE] = FALSE;
	new_exp_token = NULL;
	idx = 0;
	while (is_ifs((exp_token->value)[idx]))
		idx++;
	last_idx = idx;
	while ((exp_token->value)[idx])
	{
		if (state[IN_ESCAPE])
			state[IN_ESCAPE] = FALSE;
		else if ((exp_token->value)[idx] == '\\' && !state[IN_SQUOTE])
			state[IN_ESCAPE] = TRUE;
		else if ((exp_token->value)[idx] == '\'' && !state[IN_DQUOTE])
			state[IN_SQUOTE] = !state[IN_SQUOTE];
		else if ((exp_token->value)[idx] == '"' && !state[IN_SQUOTE])
			state[IN_DQUOTE] = !state[IN_DQUOTE];
		else
		{
			if ((!state[IN_ESCAPE] && !state[IN_SQUOTE] && !state[IN_DQUOTE])
				&& is_ifs((exp_token->value)[idx]))
			{
				new_exp_token = create_empty_exp_token();
				new_exp_token->value = ft_substr(&(exp_token->value)[last_idx], 0, idx - last_idx);
				new_exp_token->qmask = ft_substr(&(exp_token->qmask)[last_idx], 0, idx - last_idx);
				ft_lstadd_back(exp_list, ft_lstnew(new_exp_token));
				while (is_ifs(((exp_token)->value)[idx]))
					idx++;
				last_idx = idx;
				continue ;
			}
		}
		idx++;
	}
	if (last_idx != ft_strlen(exp_token->value))
	{
		new_exp_token = create_empty_exp_token();
		new_exp_token->value = ft_substr(&(exp_token->value)[last_idx], 0, idx - last_idx);
		new_exp_token->qmask = ft_substr(&(exp_token->qmask)[last_idx], 0, idx - last_idx);
		ft_lstadd_back(exp_list, ft_lstnew(new_exp_token));
	}
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:27:56 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:05:57 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "expand.h"

void	replace_exp(t_exp_token *exp_token, size_t *idx, char **env_pair)
{
	size_t	len[5];
	char	*new_exp_value;
	char	*new_exp_qmask;
	char	*suffix[2];

	len[EXP_TOKEN_LEN] = ft_strlen(exp_token->value);
	len[PARAM_LEN] = ft_strlen(env_pair[PARAM_IDX]);
	len[VALUE_LEN] = ft_strlen(env_pair[VALUE_IDX]);
	len[LEN_SUM] = len[EXP_TOKEN_LEN] + len[PARAM_LEN] + len[VALUE_LEN];
	len[SUFFIX_LEN] = len[EXP_TOKEN_LEN] - (*idx + len[PARAM_LEN]);
	suffix[0] = &(exp_token->value)[*idx + len[PARAM_LEN]];
	suffix[1] = &(exp_token->qmask)[*idx + len[PARAM_LEN]];
	new_exp_value = (char *) malloc(sizeof(char) * len[LEN_SUM]);
	new_exp_qmask = (char *) malloc(sizeof(char) * len[LEN_SUM]);
	ft_memcpy(new_exp_value, exp_token->value, *idx);
	ft_memcpy(new_exp_qmask, exp_token->qmask, *idx);
	ft_memcpy(new_exp_value + *idx, env_pair[VALUE_IDX], len[VALUE_LEN]);
	ft_memset(new_exp_qmask + *idx, '0', len[VALUE_LEN]);
	ft_memcpy(new_exp_value + *idx + len[VALUE_LEN], suffix[0], len[SUFFIX_LEN]);
	ft_memcpy(new_exp_qmask + *idx + len[VALUE_LEN], suffix[1], len[SUFFIX_LEN]);
	free(exp_token->value);
	free(exp_token->qmask);
	exp_token->value = new_exp_value;
	exp_token->qmask = new_exp_qmask;
	*idx = *idx + (ft_strlen(env_pair[VALUE_IDX]) + 1);
}

void	quote_removal(void *content)
{
	t_exp_token	*exp_token;
	int			v_idx;
	int			m_idx;

	if (!content)
		return ;
	exp_token = (t_exp_token *)content;
	v_idx = 0;
	m_idx = 0;
	while ((exp_token->qmask)[m_idx])
	{
		if ((exp_token->qmask)[m_idx] != MASK_QUOTED)
			(exp_token->value)[v_idx++] = (exp_token->value)[m_idx];
		m_idx++;
	}
	while (v_idx <= m_idx)
		(exp_token->value)[v_idx++] = '\0';
}

void	remove_exp_token(void *content)
{
	t_exp_token	*exp_token;

	exp_token = (t_exp_token *) content;
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
}

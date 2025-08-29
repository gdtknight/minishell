/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:27:56 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 06:27:31 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expand_utils.c
 * @brief Utility functions for t_exp_token manipulation
 *
 * This file contains functions to replace variables in tokens,
 * remove quotes, and free expansion token memory.
 */

#include <stdlib.h>
#include "expand.h"

/**
 * @brief Replace the token value and quote mask with new data.
 *
 * @param exp_token Token to modify
 * @param new_value New string value
 * @param new_qmask New quote mask
 */
static void	replacement(
				t_exp_token *exp_token,
				char *new_value,
				char *new_qmask);

/**
 * @brief Replace a parameter in the expansion token with its value.
 *
 * This function substitutes an environment variable in the token
 * with its actual value and updates the quote mask accordingly.
 *
 * @param exp_token Token to modify
 * @param idx Pointer to the current index in the token value
 * @param env_pair Array of [parameter, value] to replace
 */
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
	new_exp_value = (char *) ft_calloc(sizeof(char), len[LEN_SUM]);
	new_exp_qmask = (char *) ft_calloc(sizeof(char), len[LEN_SUM]);
	ft_memcpy(new_exp_value, exp_token->value, *idx);
	ft_memcpy(new_exp_qmask, exp_token->qmask, *idx);
	ft_memcpy(new_exp_value + *idx, env_pair[VALUE_IDX], len[VALUE_LEN]);
	ft_memset(new_exp_qmask + *idx, '0', len[VALUE_LEN]);
	ft_memcpy(new_exp_value + *idx + len[VALUE_LEN], \
		suffix[0], len[SUFFIX_LEN]);
	ft_memcpy(new_exp_qmask + *idx + len[VALUE_LEN], \
		suffix[1], len[SUFFIX_LEN]);
	replacement(exp_token, new_exp_value, new_exp_qmask);
	*idx = *idx + ft_strlen(env_pair[VALUE_IDX]);
}

/**
 * @brief Remove quotes from the token value based on the quote mask.
 *
 * Iterates over the quote mask and copies unquoted characters
 * into the token's value, removing all masked quote characters.
 *
 * @param content Pointer to t_exp_token
 */
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

/**
 * @brief Free an expansion token and its internal memory.
 *
 * @param content Pointer to t_exp_token
 */
void	remove_exp_token(void *content)
{
	t_exp_token	*exp_token;

	exp_token = (t_exp_token *) content;
	free(exp_token->value);
	free(exp_token->qmask);
	free(exp_token);
}

static void	replacement(
				t_exp_token *exp_token,
				char *new_value,
				char *new_qmask)
{
	free(exp_token->value);
	free(exp_token->qmask);
	exp_token->value = new_value;
	exp_token->qmask = new_qmask;
}

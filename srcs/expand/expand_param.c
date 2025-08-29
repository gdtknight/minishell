/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:29:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 07:14:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expand_param.c
 * @brief Environment variable expansion in token values.
 *
 * This module handles the expansion of shell environment variables
 * and special parameters (e.g., $?, $$, $!, $0, $*, etc.) within
 * tokens identified during shell parsing.
 */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "flag.h"
#include "utils.h"
#include "shell.h"
#include "expand.h"

/* Function prototypes for internal helpers */
static void	get_envpair(t_exp_token *exp_token, size_t idx, char ***env_pair);

/**
 * @brief Expand all environment variables in the given token.
 *
 * This function scans the token's value for dollar-sign-prefixed variables
 * and replaces them with their corresponding values from the shell
 * environment or special parameters.
 *
 * @param exp_token Token to be expanded
 * @return t_exp_token* Pointer to the expanded token
 */
t_exp_token	*expand_param(t_exp_token *exp_token)
{
	size_t	idx;
	char	**env_pair;

	env_pair = NULL;
	idx = 0;
	get_envpair(exp_token, idx, &env_pair);
	while (env_pair)
	{
		idx = find_expand_delim_pos((exp_token->value));
		replace_exp(exp_token, &idx, env_pair);
		free(env_pair[0]);
		free(env_pair[1]);
		free(env_pair);
		if (ft_strlen(exp_token->value) <= idx)
			break ;
		get_envpair(exp_token, idx, &env_pair);
	}
	return (exp_token);
}

/**
 * @brief Extract the next environment variable and its value.
 *
 * Allocates a pair of strings: the variable name and its value.
 *
 * @param exp_token Token containing the string to parse
 * @param idx Current index to start scanning
 * @param env_pair Pointer to store the resulting key-value pair
 */
static void	get_envpair(t_exp_token *exp_token, size_t idx, char ***env_pair)
{
	size_t	dollar_pos;

	*env_pair = (char **) malloc(sizeof(char *) * 2);
	if (!*env_pair)
		return ;
	(*env_pair)[PARAM_IDX] = NULL;
	(*env_pair)[VALUE_IDX] = NULL;
	dollar_pos = find_expand_delim_pos(&((exp_token->value)[idx]));
	if ((idx + dollar_pos) == ft_strlen(exp_token->value))
	{
		free(*env_pair);
		*env_pair = NULL;
		return ;
	}
	(*env_pair)[PARAM_IDX] = \
		extract_envparam(&((exp_token->value)[idx + dollar_pos]));
	if (!(*env_pair)[PARAM_IDX])
	{
		free(*env_pair);
		return ;
	}
	(*env_pair)[VALUE_IDX] = get_envvalue((*env_pair)[PARAM_IDX]);
}

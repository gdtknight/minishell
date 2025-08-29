 /* ************************************************************************** */
/*                                                                            */
//*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 06:08:25 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 07:48:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "libft.h"

#include "expand.h"

static void			get_heredoc_envpair(char *str, size_t idx, char ***env_pair);
static size_t		find_heredoc_delim_pos(char *str);
static t_boolean	check_heredoc_delim(t_boolean *flag, char *c, size_t *pos);
static void			replace_heredoc_exp(char **str, size_t *idx, char **env_pair);

void	expand_heredoc_input(char **str)
{
	size_t	idx;
	char	**env_pair;

	env_pair = NULL;
	idx = 0;
	get_heredoc_envpair(*str, idx, &env_pair);
	while (env_pair)
	{
		idx = find_heredoc_delim_pos(*str);
		replace_heredoc_exp(str, &idx, env_pair);
		free(env_pair[0]);
		free(env_pair[1]);
		free(env_pair);
		if (ft_strlen(*str) <= idx)
			break ;
		get_heredoc_envpair(*str, idx, &env_pair);
	}
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
static void	get_heredoc_envpair(char *str, size_t idx, char ***env_pair)
{
	size_t	dollar_pos;

	*env_pair = (char **) malloc(sizeof(char *) * 2);
	if (!*env_pair)
		return ;
	(*env_pair)[PARAM_IDX] = NULL;
	(*env_pair)[VALUE_IDX] = NULL;
	dollar_pos = find_heredoc_delim_pos(&(str[idx]));
	if ((idx + dollar_pos) == ft_strlen(str))
	{
		free(*env_pair);
		*env_pair = NULL;
		return ;
	}
	(*env_pair)[PARAM_IDX] = extract_envparam(&(str[idx + dollar_pos]));
	if (!(*env_pair)[PARAM_IDX])
	{
		free(*env_pair);
		return ;
	}
	(*env_pair)[VALUE_IDX] = get_envvalue((*env_pair)[PARAM_IDX]);
}

static size_t	find_heredoc_delim_pos(char *str)
{
	size_t		pos;
	char		*cur;
	t_boolean	flag[1];

	cur = str;
	flag[0] = FALSE;
	pos = 0;
	while (cur[pos])
	{
		if (check_heredoc_delim(flag, &(cur[pos]), &pos))
			return (pos);
	}
	return (pos);
}

static t_boolean	check_heredoc_delim(t_boolean *flag, char *c, size_t *pos)
{
	if (!flag[0] && *c == '\\')
	{
		flag[0] = TRUE;
		(*pos)++;
		return (FALSE);
	}
	if (!flag[0] && *c == '$' && *(c + 1) != '\0')
		return (TRUE);
	flag[0] = FALSE;
	(*pos)++;
	return (FALSE);
}

static void	replace_heredoc_exp(char **str, size_t *idx, char **env_pair)
{
	size_t	len[5];
	char	*new_str;
	char	*suffix;

	len[EXP_TOKEN_LEN] = ft_strlen(*str);
	len[PARAM_LEN] = ft_strlen(env_pair[PARAM_IDX]);
	len[VALUE_LEN] = ft_strlen(env_pair[VALUE_IDX]);
	len[LEN_SUM] = len[EXP_TOKEN_LEN] + len[PARAM_LEN] + len[VALUE_LEN];
	len[SUFFIX_LEN] = len[EXP_TOKEN_LEN] - (*idx + len[PARAM_LEN]);
	suffix = &((*str)[*idx + len[PARAM_LEN]]);
	new_str = (char *) ft_calloc(sizeof(char), len[LEN_SUM]);
	if (!new_str)
	{
		perror("ft_calloc");
		exit (EXIT_FAILURE);
	}
	ft_memcpy(new_str, *str, *idx);
	ft_memcpy(new_str + *idx, env_pair[VALUE_IDX], len[VALUE_LEN]);
	ft_memcpy(new_str + *idx + len[VALUE_LEN], suffix, len[SUFFIX_LEN]);
	*idx = *idx + ft_strlen(env_pair[VALUE_IDX]);
	free(*str);
	*str = new_str;
}

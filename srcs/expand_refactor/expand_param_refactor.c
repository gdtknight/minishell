/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param_refactor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:29:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/24 23:22:56 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

#include "flag.h"
#include "utils.h"
#include "shell.h"
#include "expand_refactor.h"

#include "debug.h"

static void	get_envpair(t_exp_token *exp_token, size_t idx, char ***env_pair);
static void	replace_exp(t_exp_token *exp_token, size_t *idx, char *env_pair[2]);
static char	*extract_envparam(char *str);
static char	*get_envvalue(const char *env_param);

t_exp_token	*expand_param_refactor(t_exp_token *exp_token)
{
	size_t	idx;
	char	**env_pair;

	env_pair = NULL;
	get_envpair(exp_token, 0, &env_pair);
	while (env_pair)
	{
		idx = find_next_delim_pos((exp_token->value), is_dollar_sign, C_BACKSLASH | C_SQUOTE);
		replace_exp(exp_token, &idx, env_pair);
		free(env_pair[0]);
		free(env_pair[1]);
		free(env_pair);
		get_envpair(exp_token, 0, &env_pair);
	}
	return (exp_token);
}

static void	get_envpair(t_exp_token *exp_token, size_t idx, char ***env_pair)
{
	size_t	dollar_pos;

	*env_pair = (char **) malloc(sizeof(char *) * 2);
	if (!*env_pair)
		return ;
	(*env_pair)[PARAM_IDX] = NULL;
	(*env_pair)[VALUE_IDX] = NULL;
	dollar_pos = find_next_delim_pos(&((exp_token->value)[idx]), is_dollar_sign, C_BACKSLASH | C_SQUOTE);
	if ((idx + dollar_pos) == ft_strlen(exp_token->value))
	{
		free(*env_pair);
		*env_pair = NULL;
		return;
	}
	(*env_pair)[PARAM_IDX] = extract_envparam(&((exp_token->value)[idx + dollar_pos]));
	if (!(*env_pair)[PARAM_IDX])
	{
		free(*env_pair);
		return;
	}
	(*env_pair)[VALUE_IDX] = get_envvalue((*env_pair)[PARAM_IDX]);
}

static void	replace_exp(t_exp_token *exp_token, size_t *idx, char **env_pair)
{
	char	*new_exp_value;
	char	*new_exp_qmask;
	size_t	new_len;
	size_t	suffix_len;

	new_len = ft_strlen(exp_token->value) - ft_strlen(env_pair[PARAM_IDX]) + ft_strlen(env_pair[VALUE_IDX]);
	new_exp_value = (char *) malloc(sizeof(char) * new_len);
	new_exp_qmask = (char *) malloc(sizeof(char) * new_len);
	suffix_len = ft_strlen(exp_token->value) - (*idx + ft_strlen(env_pair[PARAM_IDX]));
	ft_memcpy(new_exp_value, exp_token->value, *idx);
	ft_memcpy(new_exp_qmask, exp_token->qmask, *idx);
	ft_memcpy(new_exp_value + *idx, env_pair[VALUE_IDX], ft_strlen(env_pair[VALUE_IDX]));
	ft_memset(new_exp_qmask + *idx, '0', ft_strlen(env_pair[VALUE_IDX]));
	ft_memcpy(new_exp_value + *idx + ft_strlen(env_pair[VALUE_IDX]), &(exp_token->value)[*idx + ft_strlen(env_pair[PARAM_IDX])], suffix_len);
	ft_memcpy(new_exp_qmask + *idx + ft_strlen(env_pair[VALUE_IDX]), &(exp_token->qmask)[*idx + ft_strlen(env_pair[PARAM_IDX])], suffix_len);
	free(exp_token->value);
	free(exp_token->qmask);
	debug("[replace_exp] exp_token->value : \'%s\'", new_exp_value);
	debug("[replace_exp] exp_token->qmask : \'%s\'", new_exp_qmask);
	exp_token->value = new_exp_value;
	exp_token->qmask = new_exp_qmask;
	*idx = *idx + (ft_strlen(env_pair[VALUE_IDX]) + 1);
}

static char	*extract_envparam(char *str)
{
	char	*env_param;
	char	*cur;

	cur = str + 1;
	debug("[extract_envparam] cur : %s", str);
	if (ft_isdigit(*cur) || *cur == '*' || *cur == '@' || *cur == '?'
		|| *cur == '$' || *cur == '!' || (*cur == '_' && *(cur + 1) == '\0'))
		env_param = ft_substr(str, 0, 2);
	else if (ft_isalpha(*cur) || *cur == '_')
	{
		while (ft_isalnum(*cur) || *cur == '_')
			cur++;
		env_param = ft_substr(str, 0, cur - str);
	}
	else
		env_param = NULL;
	return (env_param);
}

static char	*get_envvalue(const char *env_param)
{
	t_hash_map	*map;
	char		*env_value;
	const char	*cur;

	if (!env_param)
		return (NULL);
	cur = env_param + 1;
	debug("[get_envvalue] env_param : %s", env_param);
	env_value = NULL;
	if (*cur == '?')
		env_value = (ft_itoa(get_shell_data()->last_status));
	else if (*cur == '$')
		env_value = (ft_itoa(getpid()));
	else if (*cur == '!')
	{
		if (get_shell_data()->last_bg_pid == -1)
			env_value = ft_strdup("");
		else
			env_value = ft_itoa(get_shell_data()->last_bg_pid);
	}
	else if (ft_isdigit(*cur))
		{
		if (*cur == '0')
			env_value = ft_strdup("minishell");
		else
			env_value = ft_strdup("");
	}
	else if ((*cur == '_' && *(cur + 1) == '\0')
		|| *cur == '*' || *cur == '@')
	{
		// env_value = 마지막으로 성공한 명령의 마지막 인수. 파이프 포함한 경우 제외
		env_value = ft_strdup("");
	}
	else
	{
		map = &((get_shell_data())->envp_map);
		env_value = ft_strdup(get_value(map, env_param + 1));
	}
	if (!env_value)
		return (ft_strdup(""));
	return (env_value);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:52:07 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:39:34 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"

#include "flag.h"
#include "hashmap.h"
#include "libft.h"
#include "shell_data.h"
#include "utils.h"

static char			*replace_param(char *str, char **new_value);
static char			*replace_env_key(char *str, char **new_value);
static char			*replace_single_char_param(char *str, char **new_value);
static t_boolean	is_dollar_sign(char c);

char	*expand_envp(char *value)
{
	char	*new_value;
	char	*temp;
	char	*prefix;
	char	*cur;

	new_value = NULL;
	cur = find_next_delim(value, is_dollar_sign, C_BACKSLASH | C_SQUOTE);
	while (*cur)
	{
		temp = new_value;
		prefix = ft_substr(value, 0, cur - value);
		new_value = ft_strjoin(temp, prefix);
		free(temp);
		free(prefix);
		value = replace_param(cur, &new_value);
		cur = find_next_delim(value, is_dollar_sign, C_BACKSLASH | C_SQUOTE);
	}
	temp = new_value;
	prefix = ft_substr(value, 0, cur - value);
	new_value = ft_strjoin(temp, prefix);
	free(temp);
	free(prefix);
	return (new_value);
}

static char	*replace_param(char *str, char **new_value)
{
	int		num;
	char	*temp;
	char	*cur;
	char	*env_value;

	cur = str + 1;
	if (ft_isdigit(*cur))
	{
		num = ft_atoi(cur);
		while (ft_isdigit(*(++cur)))
			;
		if (num == 0)
			env_value = ft_strdup("minishell");
		else
			env_value = ft_strdup("");
		temp = *new_value;
		(*new_value) = ft_strjoin(temp, env_value);
		free(temp);
		free(env_value);
		return (cur);
	}
	else if (*cur == '*' || *cur == '@' || *cur == '?'
		|| *cur == '$' || *cur == '!' || *cur == '_')
		return (replace_single_char_param(cur, new_value));
	else if (ft_isalpha(*cur) || *cur == '_')
		return (replace_env_key(cur, new_value));
	return (str);
}

/*
 *    `$*` : 모든 위치 매개변수(하나의 문자열로)
 *    `$@` : 모든 위치 매개변수(각각 따로, `" "`로 감쌌을 때 차이)
 *    `$?` : 직전 명령의 종료 상태값(종료 코드)
 *    `$$` : 현재 쉘 프로세스의 PID
 *    `$!` : 마지막 백그라운드 실행 프로세스의 PID
 *    `$_` : 이전 명령의 마지막 인자
 */
static char	*replace_single_char_param(char *cur, char **new_value)
{
	char	*value;
	char	*temp;

	if (*cur == '*' || *cur == '@' || *cur == '_')
		value = ft_strdup("");
	else if (*cur == '?')
		value = ft_itoa(get_shell_data()->last_status);
	else if (*cur == '$' || *cur == '!')
		value = ft_itoa(getpid());
	else
		value = NULL;
	temp = *new_value;
	*new_value = ft_strjoin(temp, value);
	free(temp);
	free(value);
	return (++cur);
}

static char	*replace_env_key(char *str, char **new_value)
{
	char	*cur;
	char	*key;
	char	*value;
	char	*temp;

	cur = str;
	while (ft_isalnum(*cur) || *cur == '_')
		cur++;
	key = ft_substr(str, 0, cur - str);
	value = get_value(&((get_shell_data())->envp_map), key);
	if (value)
		value = ft_strdup(value);
	else
		value = ft_strdup("");
	temp = *new_value;
	*new_value = ft_strjoin(temp, value);
	free(key);
	free(temp);
	free(value);
	return (cur);
}

static t_boolean	is_dollar_sign(char c)
{
	return (c == '$');
}

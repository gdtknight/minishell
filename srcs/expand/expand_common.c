/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_common.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 07:14:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 07:19:45 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "shell.h"
#include "expand.h"

/**
 * @brief Extract the environment variable name from the string.
 *
 * Handles numeric and special parameters as well as normal variable names.
 *
 * @param str String starting with '$'
 * @return char* Newly allocated string containing the variable name
 */
char	*extract_envparam(char *str)
{
	char	*env_param;
	char	*cur;

	cur = str + 1;
	if (ft_isdigit(*cur) || *cur == '*' || *cur == '@' || *cur == '?'
		|| *cur == '$' || *cur == '!' || (*cur == '_' && *(cur + 1) == '\0'))
		env_param = ft_substr(str, 0, 2);
	else if (*cur == '\"' || *cur == '\'')
		env_param = ft_strdup("$");
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

/**
 * @brief Get the value of an environment variable or special parameter.
 *
 * @param env_param Variable name starting with '$'
 * @return char* Newly allocated string containing the value
 */
char	*get_envvalue(const char *env_param)
{
	t_hash_map	*map;
	char		*env_value;

	if (!env_param)
		return (NULL);
	env_value = get_special_value(*(env_param + 1));
	if (!env_value)
	{
		map = &(get_shell_data()->envp_map);
		env_value = ft_strdup(get_value(map, env_param + 1));
	}
	if (!env_value)
		return (ft_strdup(""));
	return (env_value);
}

/**
 * @brief Get the value of special shell parameters like $? $$ $! $0 $* $@.
 *
 * @param c Character representing the special parameter
 * @return char* Newly allocated string containing the value
 */
char	*get_special_value(char c)
{
	if (c == '?')
		return (ft_itoa(get_shell_data()->last_status));
	if (c == '$')
		return (ft_itoa(getpid()));
	if (c == '!')
	{
		if ((get_shell_data())->last_bg_pid == -1)
			return (ft_strdup(""));
		else
			return (ft_itoa(get_shell_data()->last_bg_pid));
	}
	if (ft_isdigit(c))
	{
		if (c == '0')
			return (ft_strdup("minishell"));
		else
			return (ft_strdup(""));
	}
	if (c == '_' || c == '*' || c == '@')
		return (ft_strdup(""));
	return (NULL);
}

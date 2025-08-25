/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:28:58 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:20:15 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "expand.h"

t_exp_token	*expand_tilde(t_exp_token *exp_token)
{
	char	*new_value;
	char	*new_qmask;
	char	*before;
	char	*after;

	if (!exp_token || (exp_token->value)[0] != '~')
		return (exp_token);
	new_value = NULL;
	new_qmask = NULL;
	if ((exp_token->value)[1] == '\0')
	{
		after = ft_strdup(getenv("HOME"));
		new_value = ft_strdup(after);
		new_qmask = expand_mask(exp_token->qmask, 0, "~", after);
	}
	else if ((exp_token->value)[1] == '+' && (exp_token->value)[2] == '\0')
	{
		after = ft_strdup(getenv("PWD"));
		new_value = ft_strdup(after);
		new_qmask = expand_mask(exp_token->qmask, 0, "~+", after);
	}
	else if ((exp_token->value)[1] == '-' && (exp_token->value)[2] == '\0' && getenv("OLDPWD") != NULL)
	{
		after = ft_strdup(getenv("OLDPWD"));
		new_value = ft_strdup(after);
		new_qmask = expand_mask(exp_token->qmask, 0, "~-", after);
	}
	else if (ft_isalpha((exp_token->value)[1]) || (exp_token->value)[1] == '_')
	{
		before = extract_tilde_with_username(exp_token->value);
		if (!before)
			return (exp_token);
		after = get_homedir(before + 1);
		new_value = ft_strdup(after);
		new_qmask = expand_mask(exp_token->qmask, 0, before, after);
	}
	if (!new_value)
	{
		return (exp_token);
	}
	free(exp_token->value);
	free(exp_token->qmask);
	exp_token->value = new_value;
	exp_token->qmask = new_qmask;
	return (exp_token);
}

char	*extract_tilde_with_username(char *value)
{
	char	*home_prefix;
	char	*cur;
	char	*username;

	home_prefix = get_home_prefix();
	cur = value + 1;
	while (ft_isalnum(*cur) || *cur == '_')
		cur++;
	username = ft_substr(value, 0, cur - value);
	if (check_homedir(username + 1))
	{
		free(username);
		return (ft_strjoin("~", username));
	}
	free(username);
	free(home_prefix);
	return (NULL);
}

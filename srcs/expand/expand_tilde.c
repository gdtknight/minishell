/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:28:58 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 06:16:23 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"
#include "expand.h"

static char			*get_homedir(char *username);
static t_boolean	check_homedir(char *username);
static char			*get_home_prefix(void);

t_exp_token	*expand_tilde_refactor(t_exp_token *exp_token)
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

static char	*get_homedir(char *username)
{
	char	*expanded_value;
	char	*home_prefix;

	home_prefix = get_home_prefix();
	expanded_value = ft_strjoin(home_prefix, username);
	free(username);
	free(home_prefix);
	return (expanded_value);
}

static t_boolean	check_homedir(char *username)
{
	t_boolean	result;
	char		*home_prefix;
	char		*home_dir;

	result = FALSE;
	home_prefix = get_home_prefix();
	home_dir = ft_strjoin(home_prefix, username);
	if (access(home_dir, R_OK) == 0)
		result = TRUE;
	free(home_dir);
	free(home_prefix);
	return (result);
}

static char	*get_home_prefix(void)
{
	char	*home_path;
	char	*last_slash;

	home_path = getenv("HOME");
	last_slash = ft_strrchr(home_path, '/');
	return (ft_substr(
			home_path,
			0,
			ft_strlen(home_path) - ft_strlen(last_slash) + 1));
}

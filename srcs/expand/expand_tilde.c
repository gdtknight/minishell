/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:28:58 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 17:03:58 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "expand.h"

static char	*get_tilde_expansion(char *value);

t_exp_token	*expand_tilde(t_exp_token *exp_token)
{
	char	*after;
	char	*new_qmask;

	if (!exp_token || exp_token->value[0] != '~')
		return (exp_token);
	after = get_tilde_expansion(exp_token->value);
	if (!after)
		return (exp_token);
	new_qmask = expand_mask(
			exp_token->qmask,
			0,
			exp_token->value, after);
	free(exp_token->value);
	free(exp_token->qmask);
	exp_token->value = after;
	exp_token->qmask = new_qmask;
	return (exp_token);
}

char	*extract_tilde_with_username(char *value)
{
	char	*username;
	char	*tilde_with_user;
	char	*cur;

	cur = value + 1;
	while (ft_isalnum(*cur) || *cur == '_')
		cur++;
	username = ft_substr(value, 1, cur - value - 1);
	if (!username)
		return (NULL);
	if (check_homedir(username))
	{
		tilde_with_user = ft_strjoin("~", username);
		free(username);
		return (tilde_with_user);
	}
	free(username);
	return (NULL);
}

static char	*get_tilde_expansion(char *value)
{
	char	*user;
	char	*home;

	if (value[1] == '\0')
		return (ft_strdup(getenv("HOME")));
	if (value[1] == '+' && value[2] == '\0')
		return (ft_strdup(getenv("PWD")));
	if (value[1] == '-' && value[2] == '\0')
		return (ft_strdup(getenv("OLDPWD")));
	if (!ft_isalpha(value[1]) && !(value[1] == '_'))
		return (NULL);
	user = extract_tilde_with_username(value);
	if (!user)
		return (NULL);
	home = get_homedir(user + 1);
	if (!home)
	{
		free(user);
		return (NULL);
	}
	return (home);
}

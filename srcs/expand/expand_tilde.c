/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:59:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:53:21 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/unistd.h>
#include <unistd.h>

#include "def.h"
#include "libft.h"

#include "expand.h"

static char			*expand_tilde_with_user(char *value);
static t_boolean	check_home_dir(char *username);
static char			*get_home_prefix(void);

char	*expand_tilde(char *value)
{
	char	*cur;
	char	*new_value;

	if (*value != '~')
		return (ft_strdup(value));
	cur = value + 1;
	if (*cur == '\0')
		new_value = ft_strdup(getenv("HOME"));
	else if (*cur == '+' && *(cur + 1) == '\0')
		new_value = ft_strdup(getenv("PWD"));
	else if (*cur == '-' && *(cur + 1) == '\0' && getenv("OLDPWD") != NULL)
		new_value = ft_strdup((getenv("OLDPWD")));
	else if (ft_isalpha(*cur) || *cur == '_')
		new_value = expand_tilde_with_user(cur);
	else
		new_value = NULL;
	if (!new_value)
		return (ft_strdup(value));
	return (new_value);
}

static char	*expand_tilde_with_user(char *value)
{
	char	*home_prefix;
	char	*cur;
	char	*username;
	char	*expanded_value;

	expanded_value = NULL;
	home_prefix = get_home_prefix();
	cur = value;
	while (ft_isalnum(*(++cur)))
		;
	username = ft_substr(cur, 0, cur - value - 1);
	if (check_home_dir(username))
		expanded_value = ft_strjoin(home_prefix, username);
	free(username);
	free(home_prefix);
	return (expanded_value);
}

static t_boolean	check_home_dir(char *username)
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

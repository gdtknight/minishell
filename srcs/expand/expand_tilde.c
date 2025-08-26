/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:15:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 09:05:35 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expand_tilde.c
 * @brief Tilde (~) expansion for shell tokens.
 *
 * This module expands tilde expressions in tokens to their corresponding
 * home directories. Supports:
 * - ~         : current user's home directory
 * - ~username : specified user's home directory
 * - ~+        : current working directory (PWD)
 * - ~-        : previous working directory (OLDPWD)
 */

#include <stdlib.h>
#include <unistd.h>

#include "expand.h"
#include "hashmap.h"
#include "shell.h"

/* Internal helper to perform tilde expansion */
static char	*get_tilde_expansion(char *value);
static char	*get_tilde_home(void);

/**
 * @brief Expand the tilde in a token's value.
 *
 * If the token starts with '~', this function replaces it with the
 * corresponding directory (HOME, PWD, OLDPWD, or other user's home).
 * It also updates the quoting mask accordingly.
 *
 * @param exp_token Token to be expanded
 * @return t_exp_token* Pointer to the expanded token
 */
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

/**
 * @brief Extract tilde with a username, e.g., "~user".
 *
 * This function returns the string "~username" if the user exists.
 *
 * @param value String starting with '~'
 * @return char* Newly allocated string, or NULL if user not found
 */
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

/**
 * @brief Internal helper to resolve tilde expansions.
 *
 * Handles special cases like "~", "~+", "~-", or "~username".
 *
 * @param value String starting with '~'
 * @return char* Newly allocated string with expanded directory, or NULL
 */
static char	*get_tilde_expansion(char *value)
{
	char	*user;
	char	*home;

	if (value[1] == '\0')
		return (get_tilde_home());
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

static char	*get_tilde_home(void)
{
	if (contains_key(&((get_shell_data())->envp_map), \
		"HOME"))
		return (ft_strdup(get_value(&((get_shell_data())->envp_map), \
			"HOME")));
	else
		return (ft_strdup(getenv("HOME")));
}

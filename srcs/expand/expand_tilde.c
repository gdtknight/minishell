/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:59:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 21:28:18 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "def.h"
#include "utils.h"
#include "flag.h"

#include "expand.h"

static void			replace_tilde(
						char **value,
						const char *cur,
						const char *delim_pos);
static t_boolean	is_tilde(char c);

t_token	*expand_tilde(t_token *token)
{
	char		*cur;
	char		*delim_pos;
	char		*new_value;

	new_value = NULL;
	cur = token->value;
	delim_pos = find_next_delim(cur, is_tilde, C_SQUOTE | C_BACKSLASH);
	while (*cur && *delim_pos)
	{
		replace_tilde(&new_value, cur, delim_pos);
		delim_pos = find_next_delim(cur, is_tilde, C_SQUOTE | C_BACKSLASH);
		cur = delim_pos + 1;
	}
	if (new_value)
	{
		free(token->value);
		token->value = new_value;
	}
	return (token);
}

static void	replace_tilde(char **value, const char *cur, const char *delim_pos)
{
	const char	*home_dir = getenv("HOME");
	char		*prefix;
	char		*temp;

	temp = (*value);
	prefix = ft_substr(cur, 0, (delim_pos - cur));
	*value = ft_multiplejoin(temp, prefix, home_dir);
	free(temp);
	free(prefix);
}

static t_boolean	is_tilde(char c)
{
	return (c == '~');
}

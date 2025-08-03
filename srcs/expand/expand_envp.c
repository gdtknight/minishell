/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 01:00:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/03 17:45:13 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "debug.h"
#include "libft.h"

#include "def.h"
#include "utils.h"
#include "hashmap.h"
#include "shell_data.h"

#include "tokenizer.h"
#include "expand.h"

static void			replace_envp(
						char **value,
						const char *cur,
						const char *delim_pos);
static char			*extract_envp(const char *delim_pos);
static t_boolean	is_expansion(char c);

t_token	*expand_envp(t_token *token)
{
	char		*cur;
	char		*delim_pos;
	char		*new_value;
	char		*envp;

	new_value = NULL;
	cur = token->value;
	delim_pos = find_next_delim(cur, is_expansion, C_SQUOTE | C_BACKSLASH);
	while (*cur && *delim_pos)
	{
		envp = extract_envp(delim_pos);
		debug("envp: %s", envp);
		replace_envp(&new_value, cur, delim_pos);
		cur = delim_pos + ft_strlen(envp) + 1;
		delim_pos = find_next_delim(cur, is_expansion, C_SQUOTE | C_BACKSLASH);
		free(envp);
	}
	if (new_value)
	{
		free(token->value);
		token->value = new_value;
	}
	return (token);
}

static void	replace_envp(char **value, const char *cur, const char *delim_pos)
{
	char	*temp;
	char	*prefix;
	char	*envp;
	char	*envp_value;

	temp = (*value);
	prefix = ft_substr(cur, 0, (delim_pos - cur));
	envp = extract_envp(delim_pos);
	debug("envp: %s", envp);
	if (ft_strncmp(envp, "0", ft_strlen(envp)) == 0)
		envp_value = ft_strdup("minishell");
	else
	{
		envp_value = ft_strdup(get_value(&(get_shell_data())->envp_map, envp));
		if (!envp_value)
			envp_value = ft_strdup("");
	}
	debug("envp_value: %s", envp_value);
	*value = ft_multiplejoin(temp, prefix, envp_value);
	free(envp);
	free(temp);
	free(prefix);
	free(envp_value);
}

static char	*extract_envp(const char	*delim_pos)
{
	char	*end_pos;

	if (*(delim_pos + 1) == '0')
		return (ft_strdup("0"));
	end_pos = (char *)delim_pos + 1;
	while (ft_isalnum(*end_pos) || *end_pos == '_')
		end_pos++;
	return (ft_substr(delim_pos, 1, end_pos - delim_pos));
}

static t_boolean	is_expansion(char c)
{
	return (c == '$');
}

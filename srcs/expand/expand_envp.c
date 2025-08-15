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

#include "libft.h"

#include "flag.h"
#include "utils.h"

#include "expand.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 07:58:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 08:00:32 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "expand.h"

char	*extract_heredoc_envparam(char *str)
{
	char	*env_param;
	char	*cur;

	cur = str + 1;
	if (ft_isdigit(*cur) || *cur == '*' || *cur == '@' || *cur == '?'
		|| *cur == '$' || *cur == '!' || (*cur == '_' && *(cur + 1) == '\0'))
		env_param = ft_substr(str, 0, 2);
	else if (*cur == '\"' || *cur == '\'')
		env_param = NULL;
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

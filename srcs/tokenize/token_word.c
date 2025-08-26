/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 09:17:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 09:32:22 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "tokenizer.h"

char	*extract_quoted(char *str)
{
	char		*cur;

	cur = str;
	if (*cur++ == '\"')
	{
		while (*cur && *cur++ != '\"')
			;
		return (ft_substr(str, 0, cur - str));
	}
	cur--;
	if (*cur++ == '\'')
	{
		while (*cur && *cur++ != '\'')
			;
		return (ft_substr(str, 0, cur - str));
	}
	return (NULL);
}

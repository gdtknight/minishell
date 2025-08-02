/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_delim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 02:00:47 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/02 03:56:40 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "flag.h"

#include "def.h"

static void	update_flag(char c, char *flag, char mask);

char	*find_next_delim(char *str, t_boolean (*predicate)(char), char mask)
{
	char	flag;

	flag = 0;
	while (*str)
	{
		update_flag(*str, &flag, mask);
		if (!(flag & mask) && predicate(*str))
			return (str);
		str++;
	}
	return (str);
}

static void	update_flag(char c, char *flag, char mask)
{
	if (!(*flag & C_BACKSLASH))
	{
		if ((mask & C_BACKSLASH) && c == '\\')
			*flag ^= C_BACKSLASH;
		else if ((mask & C_SQUOTE) && !(*flag & C_DQUOTE) && c == '\'')
			*flag ^= C_SQUOTE;
		else if ((mask & C_DQUOTE) && !(*flag & C_SQUOTE) && c == '"')
			*flag ^= C_DQUOTE;
	}
	else
		*flag ^= C_BACKSLASH;
}

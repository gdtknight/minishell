/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 06:35:39 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 07:48:02 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static t_boolean	check_delim(t_boolean *flag, char *c, size_t *pos);

size_t	find_expand_delim_pos(char *str)
{
	size_t		pos;
	char		*cur;
	t_boolean	flag[3];

	cur = str;
	flag[0] = FALSE;
	flag[1] = FALSE;
	flag[2] = FALSE;
	pos = 0;
	while (cur[pos])
	{
		if (check_delim(flag, &(cur[pos]), &pos))
			return (pos);
	}
	return (pos);
}

static t_boolean	check_delim(t_boolean *flag, char *c, size_t *pos)
{
	if (!flag[0] && *c == '\\')
	{
		flag[0] = TRUE;
		(*pos)++;
		return (FALSE);
	}
	if (!flag[0] && !flag[1] && *c == '\'')
		flag[2] = !flag[2];
	if (!flag[0] && !flag[2] && *c == '\"')
		flag[1] = !flag[1];
	if (!flag[0] && !flag[2] && *c == '$' && *(c + 1) != '\0')
		return (TRUE);
	flag[0] = FALSE;
	(*pos)++;
	return (FALSE);
}

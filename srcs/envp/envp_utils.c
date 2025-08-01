/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:49:34 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/01 19:23:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "hashmap.h"
#include "envp.h"
#include "utils.h"

char **lst_from_hashmap(t_hash_map	*map)
{
	char			**list;
	t_hash_entry	*cur;
	int				table_idx;
	int				list_idx;

	list = (char **) malloc(sizeof(char *) * (count_entry(map) + 1));
	if (!list)
		return (NULL);
	table_idx = -1;
	list_idx = 0;
	while (++table_idx < HASHTABLE_SIZE)
	{
		cur = (map->table)[table_idx];
		while (cur)
		{
			list[list_idx++] = ft_multiplejoin(cur->key, "=", cur->value);
			cur = cur->next;
		}
	}
	return (list);
}

char	*extract_key(char *envp)
{
	char	*result;
	char	*pos;

	pos = ft_strchr(envp, '=');
	if (pos == NULL)
		return (ft_strdup(envp));
	result = ft_substr(envp, 0, ft_strlen(envp) - ft_strlen(pos));
	return (result);
}

char	*extract_value(char *envp)
{
	char	*result;
	char	*pos;

	pos = ft_strchr(envp, '=');
	if (pos == NULL)
		return (ft_strdup(""));
	result = ft_substr(pos, 1, ft_strlen(pos) - 1);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:49:34 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 22:30:45 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file env_utils.c
 * @brief Environment variable utility functions for minishell.
 *
 * This file provides helper functions to convert environment variables
 * between hash map and list representations, and to extract keys and values
 * from environment variable strings.
 */

#include <stdlib.h>

#include "libft.h"
#include "hashmap.h"
#include "utils.h"

/**
 * @brief Convert a hash map into a NULL-terminated string array.
 *
 * Iterates through all entries in the hash map and constructs a string
 * of the form "KEY=VALUE" for each entry. The resulting list is suitable
 * for passing to functions like `execve()`.
 *
 * @param map Pointer to the hash map containing environment variables.
 * @return A newly allocated NULL-terminated array of strings,
 *         or NULL on allocation failure.
 *
 * @note The caller is responsible for freeing the returned list.
 */
char	**lst_from_hashmap(t_hash_map *map)
{
	char			**list;
	t_hash_entry	*cur;
	int				table_idx;
	int				list_idx;

	list = (char **) ft_calloc(sizeof(char *), (count_entry(map) + 1));
	if (!list)
		return (NULL);
	table_idx = -1;
	list_idx = 0;
	while (++table_idx < DEFAULT_HASHTABLE_SIZE)
	{
		cur = (map->table)[table_idx];
		while (cur)
		{
			list[list_idx++] = ft_multiplejoin(cur->key, "=", cur->value);
			cur = cur->next;
		}
	}
	list[list_idx] = NULL;
	return (list);
}

/**
 * @brief Extract the key part from an environment variable string.
 *
 * Given a string in the form "KEY=VALUE", this function returns a newly
 * allocated copy of "KEY". If no '=' is present, the entire string is copied.
 *
 * @param envp The input environment variable string.
 * @return Newly allocated string containing the key.
 *
 * @note The caller must free the returned string.
 */
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

/**
 * @brief Extract the value part from an environment variable string.
 *
 * Given a string in the form "KEY=VALUE", this function returns a newly
 * allocated copy of "VALUE". If no '=' is present, it returns an empty string.
 * If the value is wrapped in single or double quotes, the quotes are removed.
 *
 * @param envp The input environment variable string.
 * @return Newly allocated string containing the value.
 *
 * @note The caller must free the returned string.
 */
char	*extract_value(char *envp)
{
	char	*result;
	char	*pos;
	char	*temp;

	pos = ft_strchr(envp, '=');
	if (pos == NULL)
		return (ft_strdup(""));
	result = ft_substr(pos, 1, ft_strlen(pos) - 1);
	if (*result == '\"')
	{
		temp = result;
		result = ft_substr(result, 1, ft_strlen(result) - 2);
		free(temp);
	}
	return (result);
}

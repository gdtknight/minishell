/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:24:17 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:26:44 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file hashmap_get.c
 * @brief Provides functions to retrieve entries or values from a hash map.
 */

#include <stdlib.h>
#include "hashmap.h"

/**
 * @brief Retrieves the hash entry corresponding to a given key.
 *
 * The key is hashed to find the corresponding bucket, and the linked list
 * in that bucket is traversed to find an entry whose key matches the given key.
 *
 * @note
 * - The key string must not be NULL.
 * - The returned entry points to internal hash map memory and must not be
 *   freed by the caller.
 *
 * @param map The hash map to search
 * @param key The key string to look up
 * @return t_hash_entry* Pointer to the hash entry, or NULL if not found
 */
t_hash_entry	*get_entry(t_hash_map *map, const char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur != NULL)
	{
		if (equal_key(cur->key, key))
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

/**
 * @brief Retrieves the value corresponding to a given key in the hash map.
 *
 * Calculates the bucket index using the hash function, then traverses
 * the linked list in that bucket to find a key that matches.
 *
 * @param map The hash map to search
 * @param key The key string to look up (must not be NULL)
 * @return char* Pointer to the value string if found,
 *               NULL if the key does not exist
 *
 * @note The returned pointer points to internal hash map memory and must
 *       not be freed.
 */
char	*get_value(t_hash_map *map, const char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur)
	{
		if (equal_key(cur->key, key))
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

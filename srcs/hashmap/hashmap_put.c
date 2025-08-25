/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_put.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:23:22 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:15:19 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file hashmap_put.c
 * @brief Provides functions to insert or update entries in a hash map.
 */

#include <stdlib.h>
#include "libft.h"
#include "hashmap.h"

/**
 * @brief Inserts or updates a key-value pair in the hash map.
 *
 * If the key already exists in the map, its value is replaced with the new value.
 * If the key does not exist, a new hash entry is created and added to the map.
 *
 * @note
 * - Both `key` and `value` should be dynamically allocated strings.
 * - Memory for key and value will be freed when `clear_hashmap()` is called.
 *
 * @param map   Pointer to the hash map
 * @param key   Key string to insert or update (dynamic allocation required)
 * @param value Value string to insert or update (dynamic allocation required)
 */
void	put_key_value(t_hash_map *map, char *key, char *value)
{
	t_hash_entry	*entry;

	entry = get_entry(map, key);
	if (entry)
	{
		if (entry->value != NULL)
			free(entry->value);
		entry->value = ft_strdup(value);
		return ;
	}
	entry = create_new_entry(key, value);
	put_entry(map, entry);
}

/**
 * @brief Inserts a hash entry into the hash map (no duplicate check).
 *
 * This function directly appends the given entry to the end of the bucket's linked list.
 * Duplicate key checking and updating should be done using `put_key_value()`.
 *
 * @note
 * - `entry->key` and `entry->value` should be dynamically allocated strings.
 * - Memory will be freed when `clear_hashmap()` is called.
 *
 * @param map   Pointer to the hash map
 * @param entry Pointer to the hash entry to insert
 */
void	put_entry(t_hash_map *map, t_hash_entry *entry)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(entry->key);
	cur = (map->table)[idx];
	if (!cur)
	{
		(map->table)[idx] = entry;
		return ;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = entry;
	entry->prev = cur;
}

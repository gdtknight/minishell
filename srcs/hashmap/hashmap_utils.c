/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:05:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:14:01 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file hashmap_utils.c
 * @brief Provides functions to remove entries and clear hash maps.
 *
 * This module includes utilities for safely removing a single entry
 * or clearing all entries from a hash map, freeing allocated memory.
 */

#include <stdlib.h>
#include "hashmap.h"

/**
 * @brief Removes a specified entry from the hash map.
 *
 * Searches for the entry in the hash map using `entry->key` and removes
 * it from the linked list of its hash bucket. Frees memory for the key,
 * value, and the entry itself.
 *
 * @param map   Pointer to the hash map
 * @param entry Pointer to the entry to remove (only the key is used)
 *
 * @note
 * - The `entry` pointer does not need to be the exact pointer stored
 *   in the hash map; only the key is required.
 * - Both key and value must be dynamically allocated strings as they
 *   will be freed inside this function.
 */
void	remove_entry(t_hash_map *map, t_hash_entry *entry)
{
	int				idx;
	t_hash_entry	*target;

	if (!map || !entry || !entry->key)
		return ;
	idx = hash(entry->key);
	target = get_entry(map, entry->key);
	if (!target)
		return ;
	if (map->table[idx] == target)
		map->table[idx] = target->next;
	if (target->prev)
		target->prev->next = target->next;
	if (target->next)
		target->next->prev = target->prev;
	free(target->key);
	free(target->value);
	free(target);
}

/**
 * @brief Clears all entries in the hash map.
 *
 * Iterates through every bucket and frees all key-value entries,
 * effectively resetting the hash map.
 *
 * @param map Pointer to the hash map to clear
 *
 * @note
 * - Keys and values inserted via `put_key_value()` or `put_entry()`
 *   must be dynamically allocated, as this function will free them.
 */
void	clear_hashmap(t_hash_map *map)
{
	int				idx;
	t_hash_entry	*cur;
	t_hash_entry	*next;

	if (!map)
		return ;
	idx = -1;
	while (++idx < DEFAULT_HASHTABLE_SIZE)
	{
		cur = map->table[idx];
		while (cur)
		{
			next = cur->next;
			if (cur->key)
				free(cur->key);
			cur->key = NULL;
			if (cur->value)
				free(cur->value);
			cur->value = NULL;
			free(cur);
			cur = NULL;
			cur = next;
		}
		map->table[idx] = NULL;
	}
}

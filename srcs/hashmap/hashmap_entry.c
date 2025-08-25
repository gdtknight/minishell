/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:05:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:14:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file hashmap_entry.c
 * @brief Provides basic hash map entry utilities: creation, counting, lookup, and hashing.
 */

#include <stdlib.h>
#include "libft.h"
#include "hashmap.h"

/**
 * @brief Creates a new hash map entry with the given key and value.
 *
 * Allocates memory for a new `t_hash_entry` and duplicates the key and value.
 * Both `next` and `prev` pointers are initialized to NULL.
 *
 * @param key   The key string for the entry (assumed not NULL)
 * @param value The value string for the entry (assumed not NULL)
 * @return Pointer to the newly created `t_hash_entry`, or NULL if allocation fails
 */
t_hash_entry	*create_new_entry(char *key, char *value)
{
	t_hash_entry	*new_entry;

	new_entry = (t_hash_entry *) malloc(sizeof(t_hash_entry));
	if (!new_entry)
		return (NULL);
	new_entry->key = ft_strdup(key);
	new_entry->value = ft_strdup(value);
	new_entry->next = NULL;
	new_entry->prev = NULL;
	return (new_entry);
}

/**
 * @brief Counts the total number of entries in the hash map.
 *
 * Iterates through all hash buckets and counts each entry in the linked list.
 *
 * @param map Pointer to the hash map
 * @return Total number of entries stored in the hash map
 */
int	count_entry(t_hash_map *map)
{
	int				cnt;
	int				idx;
	t_hash_entry	*cur;

	cnt = 0;
	idx = -1;
	while (++idx < DEFAULT_HASHTABLE_SIZE)
	{
		cur = (map->table)[idx];
		while (cur)
		{
			cnt++;
			cur = cur->next;
		}
	}
	return (cnt);
}

/**
 * @brief Checks if a given key exists in the hash map.
 *
 * Computes the hash of the key, then searches the corresponding bucket
 * for a matching key.
 *
 * @param map Pointer to the hash map
 * @param key Key string to check for existence
 * @return TRUE if the key exists, FALSE otherwise
 */
t_boolean	contains_key(t_hash_map *map, const char *key)
{
	int				idx;
	t_hash_entry	*cur;

	idx = hash(key);
	cur = (map->table)[idx];
	while (cur)
	{
		if (equal_key(cur->key, key))
			return (TRUE);
		cur = cur->next;
	}
	return (FALSE);
}

/**
 * @brief Compares two keys for equality.
 *
 * Returns TRUE if both keys are non-NULL, have the same length,
 * and have identical content.
 *
 * @param ori_key    Original key string
 * @param target_key Target key string to compare
 * @return TRUE if the keys are equal, FALSE otherwise
 */
t_boolean	equal_key(const char *ori_key, const char *target_key)
{
	if (!ori_key || !target_key)
		return (FALSE);
	return (ft_strlen(ori_key) == ft_strlen(target_key)
		&& ft_strncmp(ori_key, target_key, ft_strlen(ori_key)) == 0);
}

/**
 * @brief Computes the hash value for a given string key.
 *
 * Uses a simple hash function multiplying each character by 31 and
 * summing modulo `DEFAULT_HASHTABLE_SIZE`.
 *
 * @param key String key to hash
 * @return Hash value in the range [0, DEFAULT_HASHTABLE_SIZE)
 */
int	hash(const char *key)
{
	int	hash;

	hash = 0;
	while (*key)
		hash = ((hash + (*key++) * 31) % DEFAULT_HASHTABLE_SIZE);
	return (hash);
}

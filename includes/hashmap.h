/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:01:13 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/29 23:03:42 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H

# define HASHTABLE_SIZE		(1024)

# include "def.h"

typedef struct s_hash_entry
{
	char				*key;
	char				*value;
	struct s_hash_entry	*prev;
	struct s_hash_entry	*next;
}	t_hash_entry;

typedef struct s_hash_map
{
	t_hash_entry	*table[HASHTABLE_SIZE];
}	t_hash_map;

/* --- hashmap_clear.c --- */

void			clear_hashmap(t_hash_map *map);

/* --- hashmap_op.c --- */

void			put_key_value(t_hash_map *map, char *key, char *value);
void			put_entry(t_hash_map *map, t_hash_entry *entry);
t_hash_entry	*get_entry(t_hash_map *map, char *key);

/* --- hashmap_utils.c --- */

t_hash_entry	*create_new_entry(char *key, char *value);
t_boolean		contains_key(t_hash_map *map, char *key);
t_boolean		equal_key(char *ori_key, char *target_key);
int				hash(const char *key);

#endif

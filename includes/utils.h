/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:56:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 03:53:15 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# define INIT_CAPACITY (16)

# include <stdlib.h>

# include "def.h"
# include "hashmap.h"

typedef enum e_state_idx
{
	IN_SQUOTE = 0,
	IN_DQUOTE = 1,
	IN_ESCAPE = 2,
}	t_state_idx;

/* --- envp_utils.c --- */

char		**lst_from_hashmap(t_hash_map *map);
char		*extract_key(char *envp);
char		*extract_value(char *envp);

/* --- find_next_delim.c --- */

char		*find_next_delim(
				char *str,
				t_boolean (*predicate)(char),
				char mask);
size_t		find_next_delim_pos(
				char *str,
				t_boolean (*predicate)(char),
				char mask);
size_t		find_expand_delim_pos(char *str);

/* --- find_next_delim_utils.c --- */

t_boolean	is_ifs(char c);
t_boolean	is_dollar_sign(char c);
t_boolean	is_tk_delim(char c);
t_boolean	is_single_quote(char c);
t_boolean	is_space(int c);

/* --- ft_multiplejoin.c --- */

char		*ft_multiplejoin(const char *s1, const char *s2, const char *s3);

/* --- validate_utils.c --- */

t_boolean	is_valid_pair(char *input);

#endif

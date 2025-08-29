/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:56:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 07:04:20 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# define INIT_CAPACITY	(16)
# define LLONG_MAX_LEN	(21)

# include "def.h"
# include "hashmap.h"

typedef enum e_state_idx
{
	IN_SQUOTE = 0,
	IN_DQUOTE = 1,
	IN_ESCAPE = 2,
}	t_state_idx;

/* --- char_utils.c --- */

t_boolean	is_ifs(char c);
t_boolean	is_dollar_sign(char c);
t_boolean	is_tk_delim(char c);
t_boolean	is_single_quote(char c);
t_boolean	is_space(int c);

/* --- env_utils.c --- */

char		**lst_from_hashmap(t_hash_map *map);
char		*extract_key(char *envp);
char		*extract_value(char *envp);

/* --- ft_lltoa.c --- */

char		*ft_lltoa(long long n);

/* --- ft_multiplejoin.c --- */

char		*ft_multiplejoin(const char *s1, const char *s2, const char *s3);

/* --- pair_validator.c --- */

t_boolean	is_valid_pair(char *input);

#endif

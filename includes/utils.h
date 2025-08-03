/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:56:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/03 17:55:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "def.h"

/* --- find_next_delim.c --- */

char	*find_next_delim(
			char *str,
			t_boolean (*predicate)(char),
			unsigned char mask);

/* --- heredoc.c --- */

char	*heredoc(const char *limiter);

/* --- ft_multiplejoin.c --- */

char	*ft_multiplejoin(const char *s1, const char *s2, const char *s3);

#endif

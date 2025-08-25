/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multiplejoin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:19:46 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:17:25 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ft_multiplejoin.c
 * @brief String utility function for joining multiple strings.
 *
 * Provides a helper function to concatenate three strings into one,
 * with proper memory management.
 */

#include <stdlib.h>

#include "libft.h"
#include "utils.h"

/**
 * @brief Join three strings into a newly allocated string.
 *
 * This function concatenates `s1`, `s2`, and `s3` in order and returns
 * the result as a new null-terminated string.
 *
 * Internally, it performs two joins:
 * - First joins `s1` and `s2`.
 * - Then appends `s3` to the intermediate result.
 *
 * @param s1 First string.
 * @param s2 Second string.
 * @param s3 Third string.
 * @return A newly allocated string containing the concatenation,
 *         or NULL if memory allocation fails.
 *
 * @note The caller is responsible for freeing the returned string.
 */
char	*ft_multiplejoin(const char *s1, const char *s2, const char *s3)
{
	char	*new_str;
	char	*temp;

	new_str = NULL;
	temp = NULL;
	new_str = ft_strjoin(s1, s2);
	temp = new_str;
	new_str = ft_strjoin(temp, s3);
	free(temp);
	return (new_str);
}

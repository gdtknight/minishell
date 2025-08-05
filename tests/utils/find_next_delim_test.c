/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_delim_test.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:47:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 21:23:09 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "flag.h"
#include "utils.h"

static t_boolean	pred_dollar(char c);

void	find_next_delim_test(void)
{
	static char	*test_str = "\"abc $ def\" \\$ 'ghi $ jkl' mno $ pqr";
	char		*cursor[4];

	cursor[0] = find_next_delim(
			test_str,
			pred_dollar,
			C_BACKSLASH);
	cursor[1] = find_next_delim(
			test_str,
			pred_dollar,
			C_BACKSLASH | C_DQUOTE);
	cursor[2] = find_next_delim(
			test_str,
			pred_dollar,
			C_BACKSLASH | C_SQUOTE);
	cursor[3] = find_next_delim(
			test_str,
			pred_dollar,
			C_BACKSLASH | C_DQUOTE | C_SQUOTE);
	printf("test_str : %s\n", test_str);
	printf("set predicate to pred_dollar\n");
	printf("cursor1 (C_BACKSLASH) : %s\n", cursor[0]);
	printf("cursor2 (C_BACKSLASH | C_DQUOTE) : %s\n", cursor[1]);
	printf("cursor3 (C_BACKSLASH | C_SQUOTE) : %s\n", cursor[2]);
	printf("cursor4 (C_BACKSLASH | C_DQUOTE | C_SQUOTE) : %s\n", cursor[3]);
}

int	main(void)
{
	find_next_delim_test();
	exit(EXIT_SUCCESS);
}

static t_boolean	pred_dollar(char c)
{
	return (c == '$');
}

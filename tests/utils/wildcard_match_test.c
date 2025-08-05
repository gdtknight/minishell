/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match_test.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:37:55 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 20:58:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

static	const char	**get_strings(void)
{
	static const char	*strings[] = {
		"dev1",
		"dev2",
		"dev3",
		"dev4",
		"dev5",
		NULL
	};
	return (strings);
}

void	wildcard_match_success_test2(void)
{
	const char	*pattern = "de*3";
	const char	**strings;

	strings = get_strings();

	while (*strings)
	{
		if (wildcard_match(*strings, pattern))
			printf("Match - \"%s\" with \"%s\"\n", *strings, pattern);
		strings++;
	}
}

void	wildcard_match_success_test1(void)
{
	const char	*pattern = "*ev*";
	const char	**strings;

	strings = get_strings();

	while (*strings)
	{
		if (wildcard_match(*strings, pattern))
			printf("Match - \"%s\" with \"%s\"\n", *strings, pattern);
		strings++;
	}
}

int	main(void)
{
	wildcard_match_success_test1();
	printf("\n\n");
	wildcard_match_success_test2();
	exit(EXIT_SUCCESS);
}

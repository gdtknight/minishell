/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_delim_test.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:47:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/04 19:14:21 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "flag.h"
#include "utils.h"

static t_boolean	pred_backslash(char c);
static t_boolean	pred_dollar(char c);
static t_boolean	pred_dquote(char c);
static t_boolean	pred_squote(char c);

void	find_next_delim_test1(void)
{
	static char *test_str = "\"abc $ def\" \\$ 'ghi $ jkl' mno $ pqr";
	char				*cursor1;
	char				*cursor2;
	char				*cursor3;
	char				*cursor4;

	cursor1 = find_next_delim(test_str, pred_dollar, C_BACKSLASH);
	cursor2 = find_next_delim(test_str, pred_dollar, C_BACKSLASH | C_DQUOTE);
	cursor3 = find_next_delim(test_str, pred_dollar, C_BACKSLASH | C_SQUOTE);
	cursor4 = find_next_delim(test_str, pred_dollar, C_BACKSLASH | C_DQUOTE | C_SQUOTE);
	printf("set predicate to pred_dollar\n");
	printf("test_str : %s\n", test_str);
	printf("cursor1 (C_BACKSLASH) : %s\n", cursor1);
	printf("cursor2 (C_BACKSLASH | C_DQUOTE) : %s\n", cursor2);
	printf("cursor3 (C_BACKSLASH | C_SQUOTE) : %s\n", cursor3);
	printf("cursor4 (C_BACKSLASH | C_DQUOTE | C_SQUOTE) : %s\n", cursor4);
}

void	find_next_delim_test2(void)
{
	static char	*test_str = "\"abc $\"\\$ \'abc $ def\'";
	char				*cursor1;
	char				*cursor2;
	char				*cursor3;

	cursor1 = find_next_delim(test_str, pred_dquote, C_BACKSLASH);
	cursor2 = find_next_delim(test_str, pred_dquote, C_BACKSLASH | C_DQUOTE);
	cursor3 = find_next_delim(test_str, pred_dquote, C_BACKSLASH | C_DQUOTE | C_SQUOTE);
	printf("set predicate to pred_dquote\n");
	printf("test_str : %s\n", test_str);
	printf("cursor1  : %s\n", cursor1);
	printf("cursor2  : %s\n", cursor2);
	printf("cursor3  : %s\n", cursor3);
}

void	find_next_delim_test3(void)
{
	static char	*test_str = "\"abc $\"\\$ \'abc $ def\'";
	char				*cursor1;
	char				*cursor2;
	char				*cursor3;

	cursor1 = find_next_delim(test_str, pred_backslash, C_BACKSLASH);
	cursor2 = find_next_delim(test_str, pred_backslash, C_BACKSLASH | C_DQUOTE);
	cursor3 = find_next_delim(test_str, pred_backslash, C_BACKSLASH | C_DQUOTE | C_SQUOTE);
	printf("set predicate to pred_backslash\n");
	printf("test_str : %s\n", test_str);
	printf("cursor1  : %s\n", cursor1);
	printf("cursor2  : %s\n", cursor2);
	printf("cursor3  : %s\n", cursor3);
}

void	find_next_delim_test4(void)
{
	static char	*test_str = "\"abc $\"\\$ \'abc $ def\'";
	char				*cursor1;
	char				*cursor2;
	char				*cursor3;

	cursor1 = find_next_delim(test_str, pred_squote, C_BACKSLASH);
	cursor2 = find_next_delim(test_str, pred_squote, C_BACKSLASH | C_DQUOTE);
	cursor3 = find_next_delim(test_str, pred_squote, C_BACKSLASH | C_DQUOTE | C_SQUOTE);
	printf("set predicate to pred_squote\n");
	printf("test_str : %s\n", test_str);
	printf("cursor1  : %s\n", cursor1);
	printf("cursor2  : %s\n", cursor2);
	printf("cursor3  : %s\n", cursor3);
}

int	main(void)
{
	printf("\n\n----- find_next_delim_test1 -----\n");
	find_next_delim_test1();
	printf("\n\n----- find_next_delim_test2 -----\n");
	find_next_delim_test2();
	printf("\n\n----- find_next_delim_test3 -----\n");
	find_next_delim_test3();
	printf("\n\n----- find_next_delim_test4 -----\n");
	find_next_delim_test4();
	exit(EXIT_SUCCESS);
}

static t_boolean	pred_backslash(char c)
{
	return (c == '\\');
}

static t_boolean	pred_dollar(char c)
{
	return (c == '$');
}

static t_boolean	pred_dquote(char c)
{
	return (c == '\"');
}

static t_boolean	pred_squote(char c)
{
	return (c == '\'');
}

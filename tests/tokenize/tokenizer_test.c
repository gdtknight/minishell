/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:25:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 19:01:40 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

void	simple_test1(void)
{
	static char	*simple_str = \
		"cmd1 -option1 | cmd2 -option2 "
		"| cmd3 < infile | cmd4 > outfile";
	t_token		*tk_lst;
	t_token		*cur;
	int			cnt;

	tk_lst = tokenize_input(simple_str);
	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

void	comple_test1(void)
{
	static char	*comple_str = \
		"cmd1 & (cmd1 -option1) | cmd2 -option2 "
		"| cmd3 < infile |cmd4>outfile";
	t_token		*tk_lst;
	t_token		*cur;
	int			cnt;

	tk_lst = tokenize_input(comple_str);
	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

void	comple_test2(void)
{
	static char	*comple_str = \
		"cmd1 \"sentence 1 2 3\" && (cmd2 -option1 \'sentence 4 5 6\' \\)"
		"| cmd3 -option2 << heredoc | cat < infile "
		"|cmd4>outfile && cmd5>>appendfile";
	t_token		*tk_lst;
	t_token		*cur;
	int			cnt;

	tk_lst = tokenize_input(comple_str);
	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

void	comple_test3(void)
{
	static char	*comple_str = \
		"cmd1 \"sentence 1 2 3\"45 && (cmd2 -option1 abc\'sentence 4 5 6\' \\)"
		"| cmd3 -option2 << heredoc | cat < infile "
		"|cmd4>outfile && cmd5>>appendfile";
	t_token		*tk_lst;
	t_token		*cur;
	int			cnt;

	tk_lst = tokenize_input(comple_str);
	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

int	main(void)
{
	printf("[simple_test1]\n");
	simple_test1();
	printf("\n\n[comple_test1]\n");
	comple_test1();
	printf("\n\n[comple_test2]\n");
	comple_test2();
	printf("\n\n[comple_test3]\n");
	comple_test3();
	exit(EXIT_SUCCESS);
}

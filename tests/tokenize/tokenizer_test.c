/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:25:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:43:26 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

void	fixing_test(void)
{
	static char	*test_input = \
		"< infile.txt cmd1 -n option1 "
		"| cmd2 < infile2-1.txt < infile2-2.txt -option2-1 arg2-1 arg2-2"
		"&& cmd3 -option3 | cmd4 -option4-1 -option4-2 arg4-1 arg4-2 "
		"| (cmd5 -option5 arg5 && cmd6 -option6 >> append6.txt)";
	t_token		*tk_lst;
	t_token		*cur;
	int			cnt;

	tk_lst = tokenize_input(test_input);
	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		print_token(cur);
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

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
		print_token(cur);
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

void	comple_test1(void)
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
		print_token(cur);
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
	fixing_test();
	exit(EXIT_SUCCESS);
}

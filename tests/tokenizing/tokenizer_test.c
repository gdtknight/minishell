/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:25:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/24 14:41:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

void	simple_test1(void)
{
	char	*simple_str = "cmd1 -option1 | cmd2 -option2 | cmd3 < infile | cmd4 > outfile";

	t_token	*tk_lst = tokenize_input(simple_str);
	t_token *cur;
	int		cnt;

	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

void comple_test1()
{
	char	*comple_str = "cmd1 & (cmd1 -option1) | cmd2 -option2 | cmd3 < infile |cmd4>outfile";

	t_token	*tk_lst = tokenize_input(comple_str);
	t_token *cur;
	int		cnt;

	cur = tk_lst;
	cnt = 0;
	while (cur)
	{
		printf("[%02d] Token : %s\n", cnt++, (char *)(cur->value));
		cur = cur->next;
	}
	clear_token_lst(&tk_lst);
}

void comple_test2()
{
	char	*comple_str = "cmd1 \"sentence 1 2 3\" && (cmd2 -option1 \'sentence 4 5 6\' \\) | cmd3 -option2 << heredoc | cat < infile |cmd4>outfile && cmd5>>appendfile";

	t_token	*tk_lst = tokenize_input(comple_str);
	t_token *cur;
	int		cnt;

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
	simple_test1();
	printf("\n\n\n");
	comple_test1();
	printf("\n\n\n");
	comple_test2();
	exit(EXIT_SUCCESS);
}

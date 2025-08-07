/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validate_utils_test.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:07:50 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/07 23:17:52 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "tokenizer.h"

void	token_validate_fail_test(void)
{
	static char	*test_input = \
		"< infile.txt cmd1 -n option1 "
		"| | cmd2 < infile2-1.txt < infile2-2.txt -option2-1 arg2-1 arg2-2"
		"&& cmd3 -option3 | cmd4 -option4-1 -option4-2 arg4-1 arg4-2 "
		"| (cmd5 -option5 arg5 && cmd6 -option6 >> append6.txt)";
	t_token		*tk_lst;
	t_token		*cur;

	tk_lst = tokenize_input(test_input);
	if (!is_valid_sequence(tk_lst))
	{
		printf("[SUCCESS] invalid tk_lst\n");
		cur = tk_lst;
		while (cur)
		{
			print_token(cur);
			cur = cur->next;
		}
	}
	clear_token_lst(&tk_lst);
}

void	token_validate_success_test(void)
{
	static char	*test_input = \
		"< infile.txt cmd1 -n option1 "
		"| cmd2 < infile2-1.txt < infile2-2.txt -option2-1 arg2-1 arg2-2"
		"&& cmd3 -option3 | cmd4 -option4-1 -option4-2 arg4-1 arg4-2 "
		"| (cmd5 -option5 arg5 && cmd6 -option6 >> append6.txt)";
	t_token		*tk_lst;
	t_token		*cur;

	tk_lst = tokenize_input(test_input);
	if (is_valid_sequence(tk_lst))
	{
		printf("[SUCCESS] valid tk_lst\n");
		cur = tk_lst;
		while (cur)
		{
			print_token(cur);
			cur = cur->next;
		}
	}
	clear_token_lst(&tk_lst);
}

int	main(void)
{
	token_validate_success_test();
	printf("\n\n");
	token_validate_fail_test();
	exit(EXIT_SUCCESS);
}

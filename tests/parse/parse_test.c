/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:41:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/04 02:06:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "tokenizer.h"

static t_token	*create_test_tk_lst(void);

int	main(void)
{
	t_token			*tk_lst;
	t_token			*cur;
	t_syntax_node	*root;

	tk_lst = create_test_tk_lst();
	/* --- start test --- */
	cur = tk_lst;
	root = parse_input(&cur);
	printf("\n\n");
	print_node(root, 0);
	remove_syntax_node(root);
	/* --- end test --- */
	clear_token_lst(&tk_lst);
	exit(EXIT_SUCCESS);
}

static t_token	*create_test_tk_lst(void)
{
	static char	*test_input = \
		"< infile.txt cmd1 -n option1 | cmd2 < infile2-1.txt < infile2-2.txt -option2-1 arg2-1 arg2-2"
		"&& cmd3 -option3 | cmd4 -option4-1 -option4-2 arg4-1 arg4-2 | (cmd5 -option5 arg5 && cmd6 -option6 >> append6.txt)";
	t_token		*test_lst;

	test_lst = tokenize_input(test_input);
	return (test_lst);
}

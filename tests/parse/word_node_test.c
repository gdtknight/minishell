/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_node_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:51:51 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/24 17:07:45 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "syntax_tree.h"
#include "tokenizer.h"

static t_token	*create_test_tk_lst(void);

int	main(void)
{
	t_token	*tk_lst;

	tk_lst = create_test_tk_lst();

	/* --- start test --- */
	/* --- end test --- */

	clear_token_lst(&tk_lst);
	exit(EXIT_SUCCESS);
}

static t_token	*create_test_tk_lst(void)
{
	char	*test_input = "echo -n 123";
	t_token	*test_lst;

	test_lst = tokenize_input(test_input);
	return (test_lst);
}

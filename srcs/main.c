/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:46:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 21:34:37 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <errno.h>

#include "debug.h"

#include "shell_data.h"
#include "tokenizer.h"
#include "parser.h"

static int	interactive_mode(void);

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (!init_shell_data(envp))
		exit(errno);
	if (argc == 1)
	{
		debug("start interactive_mode");
		interactive_mode();
		exit(get_shell_data()->last_status);
	}
	exit(EXIT_SUCCESS);
}

static int	interactive_mode(void)
{
	t_token			*tk_lst;
	t_token			*tk;
	t_syntax_node	*syntax_tree;
	char			*input;

	input = readline(PROMPT);
	while (input != NULL)
	{
		debug("call tokenize_input");
		add_history(input);
		tk_lst = tokenize_input(input);
		tk = tk_lst;
		debug("call parse_input");
		syntax_tree = parse_input(&tk);
		debug("call clear_token_lst");
		clear_token_lst(&tk_lst);
		debug("call eval");
		remove_syntax_node(syntax_tree);
		// (get_shell_data())->last_status = eval(st);
		printf("current input : %s\n", input);
		input = readline(PROMPT);
	}
	return (get_shell_data()->last_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:46:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/07 23:44:27 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>

#include "debug.h"
#include "libft.h"

#include "shell_data.h"
#include "tokenizer.h"
#include "parser.h"
#include "eval.h"
#include "sig.h"

static int	interactive_mode(void);

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
    save_terminal_settings();
    init_signals();
	if (!init_shell_data(envp))
		exit(errno);
	if (argc == 1)
	{
		debug("start interactive_mode");
		interactive_mode();
	}
	restore_terminal_settings();
	exit(get_shell_data()->last_status);
}

static int	interactive_mode(void)
{
	t_token			*tk_lst;
	t_token			*tk;
	t_syntax_node	*syntax_tree;
	t_syntax_node	*st;
	char			*input;

	while (TRUE)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input == '\0' || ft_strncmp(input, "", ft_strlen(input)) == 0)
			continue ;
		add_history(input);
		tk_lst = tokenize_input(input);
		if (!is_valid_sequence(tk_lst))
		{
			printf("Invalid input\n");
			clear_token_lst(&tk_lst);
		}
		if (tk_lst)
		{
			tk = tk_lst;
			syntax_tree = parse_input(&tk);
			clear_token_lst(&tk_lst);
			st = syntax_tree;
			(get_shell_data())->last_status = eval(st);
			debug("current input : %s\n", input);
			remove_syntax_node(syntax_tree);
		}
	}
	// set_terminal_print_on();
	return (get_shell_data()->last_status);
}

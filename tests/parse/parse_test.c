/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:41:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/27 00:57:38 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "syntax_tree.h"
#include "tokenizer.h"

static t_token	*create_test_tk_lst(void);
static	void	print_node(t_syntax_node *node, int depth);

int	main(void)
{
	t_token			*tk_lst;
	t_syntax_node	*root;

	tk_lst = create_test_tk_lst();

	/* --- start test --- */
	root = shell_input(&tk_lst);
	printf("\n\n");
	print_node(root, 0);
	remove_syntax_node(root);
	/* --- end test --- */

	clear_token_lst(&tk_lst);
	free(tk_lst);
	exit(EXIT_SUCCESS);
}

static t_token	*create_test_tk_lst(void)
{
	char	*test_input = "< infile.txt echo -n 123 | cat infile.txt && cmd1 -option1 | cmd2 -option2";
	t_token	*test_lst;

	test_lst = tokenize_input(test_input);
	return (test_lst);
}

static	void	print_node(t_syntax_node *node, int depth)
{
	debug("print_node called - depth : %02d", depth);
	if (!node || depth > 10)
		return ;
	debug("check_node type");
	if (node->type == NODE_LIST)
	{
		printf("[%02d] list_node - %s\n", depth, node->value.b_operator.operator);
		print_node(node->value.b_operator.left, depth + 1);
		print_node(node->value.b_operator.right, depth + 1);
	}
	else if (node->type == NODE_AND_OR)
	{
		printf("[%02d] and_or_node - %s\n", depth, node->value.b_operator.operator);
		print_node(node->value.b_operator.left, depth + 1);
		print_node(node->value.b_operator.right, depth + 1);
	}
	else if (node->type == NODE_PIPELINE || node->type == NODE_PIPELINE_ERR)
	{
		printf("[%02d] pipeline_node - %s\n", depth, node->value.b_operator.operator);
		print_node(node->value.b_operator.left, depth + 1);
		print_node(node->value.b_operator.right, depth + 1);
	}
	else if (node->type == NODE_SIMPLE_COMMAND)
	{
		printf("[%02d] simple_command_node - %s\n", depth, "simple_command");
		print_node(node->value.command.prefix, depth + 1);
		print_node(node->value.command.word, depth + 1);
		print_node(node->value.command.suffix, depth + 1);
	}
	else if (node->type == NODE_CMD_PREFIX)
	{
		printf("[%02d] cmd_prefix_node - %s\n", depth, "cmd_prefix");
		print_node(node->value.b_node.left, depth + 1);
		print_node(node->value.b_node.right, depth + 1);
	}
	else if (node->type == NODE_CMD_WORD)
	{
		printf("[%02d] cmd_word_node - %s\n", depth, "cmd_word");
		print_node(node->value.child, depth + 1);
	}
	else if (node->type == NODE_CMD_SUFFIX)
	{
		printf("[%02d] cmd_suffix_node - %s\n", depth, "cmd_suffix");
		print_node(node->value.b_node.left, depth + 1);
		print_node(node->value.b_node.right, depth + 1);
	}
	else if (node->type == NODE_ASSIGN_WORD)
	{
		printf("[%02d] assign_word_node - %s\n", depth, "assign_word");
		print_node(node->value.child, depth + 1);
	}
	else if (node->type == NODE_IO_REDIR_IN
			|| node->type == NODE_IO_REDIR_OUT
			|| node->type == NODE_IO_REDIR_HEREDOC
			|| node->type == NODE_IO_REDIR_APPEND)
	{
		printf("[%02d] io_redir_node - %s\n", depth, node->value.u_operator.operator);
		print_node(node->value.u_operator.target, depth + 1);
	}
	else if (node->type == NODE_WORD)
		printf("[%02d] word_node - %s\n", depth, node->value.word);
	else
		return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:15:05 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/04 17:58:23 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "tokenizer.h"
#include "parser.h"

/*
 * <command>        ::= <simple_command>
 *                    | <compound_command>
 *                    | <function_def>
 *
 * <simple_command> ::= <cmd_prefix> <cmd_word> <cmd_suffix>
 *                    | <cmd_word> <cmd_suffix>
 *                    | <cmd_prefix> <cmd_word>
 *                    | <cmd_word>
 * 
 * 
 * <assignment_word>::= <name> "=" <word>
 */
t_syntax_node	*command(t_token **tk_lst)
{
	t_syntax_node	*command_node;

	if ((*tk_lst)->type == TK_EOF)
		return (NULL);
	if ((*tk_lst)->type == TK_LPAREN)
	{
		command_node = create_empty_node();
		command_node->type = NODE_COMPOUND_COMMAND;
		(*tk_lst) = (*tk_lst)->next;
		command_node->value.child = list(tk_lst);
		if ((*tk_lst)->type != TK_RPAREN)
		{
			/* Error !!! */
			return (NULL);
		}
		(*tk_lst) = (*tk_lst)->next;
	}
	else
		command_node = simple_command(tk_lst);
	return (command_node);
}

t_syntax_node	*simple_command(t_token **tk_lst)
{
	t_syntax_node	*simple_command_node;

	if ((*tk_lst)->type == TK_EOF)
		return (NULL);
	simple_command_node = create_empty_node();
	simple_command_node->type = NODE_SIMPLE_COMMAND;
	simple_command_node->value.command.prefix = cmd_prefix(tk_lst);
	simple_command_node->value.command.word = ft_strdup((*tk_lst)->value);
	(*tk_lst) = (*tk_lst)->next;
	simple_command_node->value.command.suffix = cmd_suffix(tk_lst);
	return (simple_command_node);
}

/*
 * <cmd_prefix>     ::= <io_redirect>
 *                    | <cmd_prefix> <io_redirect>
 *                    | <assignment_word>
 *                    | <cmd_prefix> <assignment_word>
 */

t_syntax_node	*cmd_prefix(t_token **tk_lst)
{
	t_syntax_node	*cmd_prefix_node;
	t_syntax_node	*temp;

	if ((*tk_lst)->type == TK_EOF)
		return (NULL);
	cmd_prefix_node = io_redir(tk_lst);
	if (cmd_prefix_node == NULL)
		cmd_prefix_node = assignment_word(tk_lst);
	if (cmd_prefix_node == NULL)
		return (NULL);
	while ((*tk_lst)->type == TK_REDIR_IN || (*tk_lst)->type == TK_REDIR_OUT
		|| (*tk_lst)->type == TK_REDIR_HEREDOC || (*tk_lst)->type == TK_REDIR_APPEND
		|| (*tk_lst)->type == TK_ASSIGN_WORD)
	{
		temp = cmd_prefix_node;
		cmd_prefix_node = create_empty_node();
		cmd_prefix_node->type = NODE_CMD_PREFIX;
		cmd_prefix_node->value.b_node.left = temp;
		cmd_prefix_node->value.b_node.right = io_redir(tk_lst);
		if (cmd_prefix_node->value.b_node.right == NULL)
			cmd_prefix_node->value.b_node.right = assignment_word(tk_lst);
	}
	return (cmd_prefix_node);
}

/*
 * <cmd_suffix>     ::= <io_redirect>
 *                    | <cmd_suffix> <io_redirect>
 *                    | <word>
 *                    | <cmd_suffix> <word>
 */
t_syntax_node	*cmd_suffix(t_token **tk_lst)
{
	t_syntax_node	*cmd_suffix_node;
	t_syntax_node	*temp;

	cmd_suffix_node = io_redir(tk_lst);
	if (cmd_suffix_node == NULL)
		cmd_suffix_node = word(tk_lst);
	if (cmd_suffix_node == NULL)
		return (NULL);
	while ((*tk_lst)->type == TK_REDIR_IN
		|| (*tk_lst)->type == TK_REDIR_OUT
		|| (*tk_lst)->type == TK_REDIR_HEREDOC
		|| (*tk_lst)->type == TK_REDIR_APPEND
		|| (*tk_lst)->type == TK_WORD)
	{
		temp = cmd_suffix_node;
		cmd_suffix_node = create_empty_node();
		cmd_suffix_node->type = NODE_CMD_SUFFIX;
		cmd_suffix_node->value.b_node.left = temp;
		cmd_suffix_node->value.b_node.right = io_redir(tk_lst);
		if (cmd_suffix_node->value.b_node.right == NULL)
			cmd_suffix_node->value.b_node.right = word(tk_lst);
	}
	return (cmd_suffix_node);
}

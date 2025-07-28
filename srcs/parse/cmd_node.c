/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:15:05 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 14:54:35 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

#include "syntax_tree.h"
#include "tokenizer.h"

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
	debug("command called - %s", (char *)((*tk_lst)->value));
	if ((*tk_lst)->type == TK_EOF)
	{
		debug("TK_EOF");
		return (NULL);
	}
	return (simple_command(tk_lst));
}

t_syntax_node	*simple_command(t_token **tk_lst)
{
	t_syntax_node	*simple_command_node;

	debug("simple_command called - %s", (char *)((*tk_lst)->value));
	if ((*tk_lst)->type == TK_EOF)
	{
		debug("TK_EOF");
		return (NULL);
	}
	simple_command_node = create_empty_node();
	simple_command_node->type = NODE_SIMPLE_COMMAND;
	simple_command_node->value.command.prefix = cmd_prefix(tk_lst);
	simple_command_node->value.command.word = cmd_word(tk_lst);
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

	debug("cmd_prefix called - %s", (char *)((*tk_lst)->value));
	if ((*tk_lst)->type == TK_EOF)
	{
		debug("TK_EOF");
		return (NULL);
	}
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

/* --- <cmd_word> ::= <word> --- */

t_syntax_node	*cmd_word(t_token **tk_lst)
{
	t_syntax_node	*cmd_word;

	if ((*tk_lst)->type == TK_EOF)
	{
		debug("TK_EOF");
		return (NULL);
	}
	debug("cmd_word called - %s", (char *)((*tk_lst)->value));
	cmd_word = create_empty_node();
	cmd_word->type = NODE_CMD_WORD;
	cmd_word->value.child = word(tk_lst);
	return (cmd_word);
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

	debug("cmd_suffix called - %s", (char *)((*tk_lst)->value));
	cmd_suffix_node = io_redir(tk_lst);
	if (cmd_suffix_node == NULL)
		cmd_suffix_node = word(tk_lst);
	if (cmd_suffix_node == NULL)
		return (NULL);
	while ((*tk_lst)->type == TK_REDIR_IN
		|| (*tk_lst)->type == TK_REDIR_OUT
		|| (*tk_lst)->type == TK_REDIR_HEREDOC
		|| (*tk_lst)->type == TK_REDIR_APPEND
		|| (*tk_lst)->type == TK_WORD
		|| (*tk_lst)->type == TK_WORD_WITH_DQUOTE
		|| (*tk_lst)->type == TK_WORD_WITH_SQUOTE)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:15:05 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 02:02:08 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"

#include "tokenizer.h"
#include "ast.h"

/**
 * @brief Parses a command node from the token list.
 *
 * If the current token is a left parenthesis, parses a compound command node.
 * Otherwise, parses a simple command node.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The created command node, or NULL on failure.
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
		command_node->value.child->parent = command_node;
		if ((*tk_lst)->type != TK_RPAREN)
			return (NULL);
		(*tk_lst) = (*tk_lst)->next;
	}
	else
		command_node = simple_command(tk_lst);
	return (command_node);
}

/**
 * @brief Parses a simple command node from the token list.
 *
 * Handles prefix, command word, and suffix for a simple command.
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The created simple command node, or NULL on failure.
 */
t_syntax_node	*simple_command(t_token **tk_lst)
{
	t_syntax_node	*simple_command_node;

	if ((*tk_lst)->type == TK_EOF)
		return (NULL);
	simple_command_node = create_empty_node();
	simple_command_node->type = NODE_SIMPLE_COMMAND;
	simple_command_node->value.command.form.cmd = NULL;
	simple_command_node->value.command.form.args = NULL;
	simple_command_node->value.command.form.envp = NULL;
	simple_command_node->value.command.heredoc_fds[PIPE_READ] = -1;
	simple_command_node->value.command.heredoc_fds[PIPE_WRITE] = -1;
	simple_command_node->value.command.prefix = cmd_prefix(tk_lst);
	if (simple_command_node->value.command.prefix)
		simple_command_node->value.command.prefix->parent = simple_command_node;
	simple_command_node->value.command.cmd_word = word(tk_lst);
	simple_command_node->value.command.suffix = cmd_suffix(tk_lst);
	if (simple_command_node->value.command.suffix)
		simple_command_node->value.command.suffix->parent = simple_command_node;
	return (simple_command_node);
}

/**
 * @brief Parses the <cmd_prefix> rule and creates a syntax tree node.
 *
 * <cmd_prefix> ::= <io_redirect>
 *               | <cmd_prefix> <io_redirect>
 *               | <assignment_word>
 *               | <cmd_prefix> <assignment_word>
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The created prefix node, or NULL on failure.
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
		|| (*tk_lst)->type == TK_REDIR_HEREDOC
		|| (*tk_lst)->type == TK_REDIR_APPEND
		|| (*tk_lst)->type == TK_ASSIGN_WORD)
	{
		temp = cmd_prefix_node;
		cmd_prefix_node = create_empty_node();
		cmd_prefix_node->type = NODE_CMD_PREFIX;
		cmd_prefix_node->value.b_node.left = temp;
		cmd_prefix_node->value.b_node.left->parent = cmd_prefix_node;
		cmd_prefix_node->value.b_node.right = cmd_prefix(tk_lst);
		if (cmd_prefix_node->value.b_node.right)
			cmd_prefix_node->value.b_node.right->parent = cmd_prefix_node;
	}
	return (cmd_prefix_node);
}

/**
 * @brief Parses the <cmd_suffix> rule and creates a syntax tree node.
 *
 * <cmd_suffix> ::= <io_redirect>
 *               | <cmd_suffix> <io_redirect>
 *               | <word>
 *               | <cmd_suffix> <word>
 *
 * @param tk_lst Pointer to the current token list position.
 * @return t_syntax_node* The created suffix node, or NULL on failure.
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
		cmd_suffix_node->value.b_node.left->parent = cmd_suffix_node;
		cmd_suffix_node->value.b_node.right = cmd_suffix(tk_lst);
		if (cmd_suffix_node->value.b_node.right)
			cmd_suffix_node->value.b_node.right->parent = cmd_suffix_node;
	}
	return (cmd_suffix_node);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redir_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 22:10:43 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 14:49:25 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

#include "libft.h"

#include "syntax_tree.h"
#include "tokenizer.h"

static t_syntax_node	*io_redir_out(t_token **tk_lst);
static t_syntax_node	*io_redir_append(t_token **tk_lst);
static t_syntax_node	*io_redir_in(t_token **tk_lst);
static t_syntax_node	*io_redir_heredoc(t_token **tk_lst);

/*
 * <io_redirect>    ::= [<number>]">" <word>
 *                    | [<number>]">>" <word>
 *                    | [<number>]"<" <word>
 *                    | [<number>]"<<" <word>
 *                    | [<number>]"<>" <word> (x)
 *
 * [<number>] : 파일디스크립터. 여기선 무시하고 구현
 */
t_syntax_node	*io_redir(t_token **tk_lst)
{
	t_syntax_node	*io_redir;
	t_token_type	type;

	debug("io_redir called - %s", (char *)((*tk_lst)->value));
	if (!*tk_lst)
		return (NULL);
	io_redir = NULL;
	type = (*tk_lst)->type;
	if (type == TK_REDIR_OUT)
		io_redir = io_redir_out(tk_lst);
	if (type == TK_REDIR_APPEND)
		io_redir = io_redir_append(tk_lst);
	if (type == TK_REDIR_IN)
		io_redir = io_redir_in(tk_lst);
	if (type == TK_REDIR_HEREDOC)
		io_redir = io_redir_heredoc(tk_lst);
	return (io_redir);
}

static t_syntax_node	*io_redir_out(t_token **tk_lst)
{
	t_syntax_node	*io_redir_out_node;

	debug("io_redir_out called");
	io_redir_out_node = create_empty_node();
	io_redir_out_node->type = NODE_IO_REDIR_OUT;
	io_redir_out_node->value.u_operator.operator = ft_strdup(STR_REDIR_OUT);
	(*tk_lst) = (*tk_lst)->next;
	io_redir_out_node->value.u_operator.target = word(tk_lst);
	return (io_redir_out_node);
}

static t_syntax_node	*io_redir_append(t_token **tk_lst)
{
	t_syntax_node	*io_redir_append_node;

	debug("io_redir_append called");
	io_redir_append_node = create_empty_node();
	io_redir_append_node->type = NODE_IO_REDIR_APPEND;
	io_redir_append_node->value.u_operator.operator = \
		ft_strdup(STR_REDIR_APPEND);
	(*tk_lst) = (*tk_lst)->next;
	io_redir_append_node->value.u_operator.target = word(tk_lst);
	return (io_redir_append_node);
}

static t_syntax_node	*io_redir_in(t_token **tk_lst)
{
	t_syntax_node	*io_redir_in_node;

	debug("io_redir_in called");
	io_redir_in_node = create_empty_node();
	io_redir_in_node->type = NODE_IO_REDIR_IN;
	io_redir_in_node->value.u_operator.operator = ft_strdup(STR_REDIR_IN);
	(*tk_lst) = (*tk_lst)->next;
	io_redir_in_node->value.u_operator.target = word(tk_lst);
	return (io_redir_in_node);
}

static t_syntax_node	*io_redir_heredoc(t_token **tk_lst)
{
	t_syntax_node	*io_redir_heredoc_node;

	debug("io_redir_heredoc called");
	io_redir_heredoc_node = create_empty_node();
	io_redir_heredoc_node->type = NODE_IO_REDIR_HEREDOC;
	io_redir_heredoc_node->value.u_operator.operator = \
		ft_strdup(STR_REDIR_HEREDOC);
	(*tk_lst) = (*tk_lst)->next;
	io_redir_heredoc_node->value.u_operator.target = word(tk_lst);
	return (io_redir_heredoc_node);
}

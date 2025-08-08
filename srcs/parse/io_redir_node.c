/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redir_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 22:10:43 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 19:35:22 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "tokenizer.h"

#include "parser.h"

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

	if (!*tk_lst)
		return (NULL);
	io_redir = (NULL);
	type = (*tk_lst)->type;
	if (type == TK_REDIR_OUT)
		io_redir = io_redir_out(tk_lst);
	else if (type == TK_REDIR_APPEND)
		io_redir = io_redir_append(tk_lst);
	else if (type == TK_REDIR_IN)
		io_redir = io_redir_in(tk_lst);
	else if (type == TK_REDIR_HEREDOC)
		io_redir = io_redir_heredoc(tk_lst);
	else
		return (NULL);
	(*tk_lst) = (*tk_lst)->next;
	return (io_redir);
}

static t_syntax_node	*io_redir_out(t_token **tk_lst)
{
	t_syntax_node	*io_redir_out_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_out_node = create_empty_node();
	io_redir_out_node->type = NODE_IO_REDIR_OUT;
	io_redir_out_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_out_node);
}

static t_syntax_node	*io_redir_append(t_token **tk_lst)
{
	t_syntax_node	*io_redir_append_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_append_node = create_empty_node();
	io_redir_append_node->type = NODE_IO_REDIR_APPEND;
	io_redir_append_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_append_node);
}

static t_syntax_node	*io_redir_in(t_token **tk_lst)
{
	t_syntax_node	*io_redir_in_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_in_node = create_empty_node();
	io_redir_in_node->type = NODE_IO_REDIR_IN;
	io_redir_in_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_in_node);
}

static t_syntax_node	*io_redir_heredoc(t_token **tk_lst)
{
	t_syntax_node	*io_redir_heredoc_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_heredoc_node = create_empty_node();
	io_redir_heredoc_node->type = NODE_IO_REDIR_HEREDOC;
	io_redir_heredoc_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_heredoc_node);
}

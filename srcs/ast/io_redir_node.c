/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redir_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 22:10:43 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/18 01:44:44 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "tokenizer.h"

#include "ast.h"

static t_syntax_node	*io_redir_out(t_token **tk_lst);
static t_syntax_node	*io_redir_append(t_token **tk_lst);
static t_syntax_node	*io_redir_in(t_token **tk_lst);
static t_syntax_node	*io_redir_heredoc(t_token **tk_lst);

/**
 * @brief <io_redirect> 규칙을 파싱하여 구문 트리 노드를 생성한다.
 *
 * <io_redirect> ::= [<number>] ">"  <word>
 *                 | [<number>] ">>" <word>
 *                 | [<number>] "<"  <word>
 *                 | [<number>] "<<" <word>
 *                 | [<number>] "<>" <word> (미구현)
 *
 * @param tk_lst 현재 파싱 위치를 나타내는 토큰 리스트 포인터
 * @return t_syntax_node* 생성된 I/O 리다이렉션 노드, 실패 시 NULL
 *
 * @note
 * - [<number>]는 파일 디스크립터를 의미하지만, 현재 구현에서는 무시한다.
 * - 토큰 타입(TK_REDIR_*)에 따라 각 전용 함수(io_redir_out 등)를 호출.
 * - 생성된 노드의 value.io_target에는 리다이렉션 대상 경로가 문자열로 저장된다.
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

/**
 * @brief 출력 리다이렉션 '>' 노드를 생성한다.
 *
 * @param tk_lst 현재 파싱 위치의 토큰 리스트 포인터
 * @return t_syntax_node* NODE_IO_REDIR_OUT 타입의 노드
 *
 * @note
 * - 현재 토큰('>')을 건너뛰고, 다음 토큰을 io_target으로 설정.
 */
static t_syntax_node	*io_redir_out(t_token **tk_lst)
{
	t_syntax_node	*io_redir_out_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_out_node = create_empty_node();
	io_redir_out_node->type = NODE_IO_REDIR_OUT;
	io_redir_out_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_out_node);
}

/**
 * @brief 출력 추가 리다이렉션 '>>' 노드를 생성한다.
 *
 * @param tk_lst 현재 파싱 위치의 토큰 리스트 포인터
 * @return t_syntax_node* NODE_IO_REDIR_APPEND 타입의 노드
 *
 * @note
 * - 현재 토큰('>>')을 건너뛰고, 다음 토큰을 io_target으로 설정.
 */
static t_syntax_node	*io_redir_append(t_token **tk_lst)
{
	t_syntax_node	*io_redir_append_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_append_node = create_empty_node();
	io_redir_append_node->type = NODE_IO_REDIR_APPEND;
	io_redir_append_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_append_node);
}

/**
 * @brief 입력 리다이렉션 '<' 노드를 생성한다.
 *
 * @param tk_lst 현재 파싱 위치의 토큰 리스트 포인터
 * @return t_syntax_node* NODE_IO_REDIR_IN 타입의 노드
 *
 * @note
 * - 현재 토큰('<')을 건너뛰고, 다음 토큰을 io_target으로 설정.
 */
static t_syntax_node	*io_redir_in(t_token **tk_lst)
{
	t_syntax_node	*io_redir_in_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_in_node = create_empty_node();
	io_redir_in_node->type = NODE_IO_REDIR_IN;
	io_redir_in_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_in_node);
}

/**
 * @brief 히어독 리다이렉션 '<<' 노드를 생성한다.
 *
 * @param tk_lst 현재 파싱 위치의 토큰 리스트 포인터
 * @return t_syntax_node* NODE_IO_REDIR_HEREDOC 타입의 노드
 *
 * @note
 * - 현재 토큰('<<')을 건너뛰고, 다음 토큰을 io_target으로 설정.
 * - 히어독 종료 구분자는 io_target에 저장된다.
 */
static t_syntax_node	*io_redir_heredoc(t_token **tk_lst)
{
	t_syntax_node	*io_redir_heredoc_node;

	(*tk_lst) = (*tk_lst)->next;
	io_redir_heredoc_node = create_empty_node();
	io_redir_heredoc_node->type = NODE_IO_REDIR_HEREDOC;
	io_redir_heredoc_node->value.io_target = ft_strdup((*tk_lst)->value);
	return (io_redir_heredoc_node);
}

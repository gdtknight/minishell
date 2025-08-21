/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:15:05 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 09:48:12 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"

#include "tokenizer.h"
#include "ast.h"

/**
 * @brief <command> 규칙을 파싱하여 구문 트리 노드를 생성한다.
 *
 * <command> ::= <simple_command>
 *             | <compound_command>
 *             | <function_def> (미구현)
 *
 * @param tk_lst 현재 파싱 위치를 나타내는 토큰 리스트 포인터
 * @return t_syntax_node* 생성된 command 노드, 실패 시 NULL
 *
 * @note
 * - '(' 로 시작하면 NODE_COMPOUND_COMMAND를 생성하고 list()로 하위 파싱을 수행.
 * - 그렇지 않으면 simple_command()로 처리.
 * - ')' 가 누락되면 NULL 반환.
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
 * @brief <simple_command> 규칙을 파싱하여 구문 트리 노드를 생성한다.
 *
 * <simple_command> ::= <cmd_prefix> <cmd_word> <cmd_suffix>
 *                     | <cmd_word> <cmd_suffix>
 *                     | <cmd_prefix> <cmd_word>
 *                     | <cmd_word>
 *
 * @param tk_lst 현재 파싱 위치를 나타내는 토큰 리스트 포인터
 * @return t_syntax_node* NODE_SIMPLE_COMMAND 노드, 실패 시 NULL
 *
 * @note
 * - prefix는 cmd_prefix()로 파싱.
 * - word는 현재 토큰의 value를 복사(ft_strdup)하여 저장.
 * - suffix는 cmd_suffix()로 파싱.
 */
t_syntax_node	*simple_command(t_token **tk_lst)
{
	t_syntax_node	*simple_command_node;

	if ((*tk_lst)->type == TK_EOF)
		return (NULL);
	simple_command_node = create_empty_node();
	simple_command_node->type = NODE_SIMPLE_COMMAND;
	simple_command_node->value.command.heredoc_fds[PIPE_READ] = -1;
	simple_command_node->value.command.heredoc_fds[PIPE_WRITE] = -1;
	simple_command_node->value.command.prefix = cmd_prefix(tk_lst);
	if (simple_command_node->value.command.prefix)
		simple_command_node->value.command.prefix->parent = simple_command_node;
	simple_command_node->value.command.word = ft_strdup((*tk_lst)->value);
	(*tk_lst) = (*tk_lst)->next;
	simple_command_node->value.command.suffix = cmd_suffix(tk_lst);
	if (simple_command_node->value.command.suffix)
		simple_command_node->value.command.suffix->parent = simple_command_node;
	simple_command_node->value.command.form.cmd = NULL;
	simple_command_node->value.command.form.args = NULL;
	simple_command_node->value.command.form.envp = NULL;
	return (simple_command_node);
}

/**
 * @brief <cmd_prefix> 규칙을 파싱하여 구문 트리 노드를 생성한다.
 *
 * <cmd_prefix> ::= <io_redirect>
 *                 | <cmd_prefix> <io_redirect>
 *                 | <assignment_word>
 *                 | <cmd_prefix> <assignment_word>
 *
 * @param tk_lst 현재 파싱 위치를 나타내는 토큰 리스트 포인터
 * @return t_syntax_node* 생성된 prefix 노드, 실패 시 NULL
 *
 * @note
 * - 최초 노드는 io_redir() 또는 assignment_word()로 파싱.
 * - 이후 연속되는 TK_REDIR_* 또는 TK_ASSIGN_WORD가 있으면
 *   NODE_CMD_PREFIX로 묶어 왼쪽·오른쪽 자식으로 연결.
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
 * @brief <cmd_suffix> 규칙을 파싱하여 구문 트리 노드를 생성한다.
 *
 * <cmd_suffix> ::= <io_redirect>
 *                 | <cmd_suffix> <io_redirect>
 *                 | <word>
 *                 | <cmd_suffix> <word>
 *
 * @param tk_lst 현재 파싱 위치를 나타내는 토큰 리스트 포인터
 * @return t_syntax_node* 생성된 suffix 노드, 실패 시 NULL
 *
 * @note
 * - 최초 노드는 io_redir() 또는 word()로 파싱.
 * - 이후 연속되는 TK_REDIR_* 또는 TK_WORD가 있으면
 *   NODE_CMD_SUFFIX로 묶어 왼쪽·오른쪽 자식으로 연결.
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

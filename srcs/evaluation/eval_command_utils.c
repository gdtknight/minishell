/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 22:17:29 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 10:15:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "ast.h"
#include "eval.h"

static int	count_args(t_syntax_node *cmd_suffix);

/**
 * @brief 명령어 suffix 노드에서 인자(argv) 배열을 생성한다.
 *
 * NODE_CMD_SUFFIX로 연결된 트리를 순회하며 NODE_WORD 타입의 값을
 * 인자로 추가하고, 마지막에 NULL 포인터로 배열을 종료한다.
 * argv[0]은 호출부에서 명령어로 설정한다.
 *
 * @param cmd_suffix 명령어 suffix를 나타내는 AST 노드
 * @return char** NULL 종료된 인자 문자열 배열(동적 할당)
 *
 * @note
 * - count_args()를 이용해 배열 크기를 미리 계산하여 메모리 할당.
 * - 반환된 배열과 내부 문자열은 호출자가 free()로 해제해야 한다.
 * - prefix의 인자는 포함하지 않는다.
 */
char	**get_args_from_suffix(t_syntax_node *cmd_suffix)
{
	t_syntax_node	*cur_node;
	char			**args;
	int				idx;

	args = (char **) malloc(sizeof(char *) * (count_args(cmd_suffix) + 2));
	cur_node = cmd_suffix;
	idx = 1;
	while (cur_node && cur_node->type == NODE_CMD_SUFFIX)
	{
		if (cur_node->value.b_node.left->type == NODE_WORD)
			args[idx++] = ft_strdup(cur_node->value.b_node.left->value.word);
		cur_node = cur_node->value.b_node.right;
	}
	if (cur_node && cur_node->type == NODE_WORD)
		args[idx++] = ft_strdup(cur_node->value.word);
	args[idx++] = (NULL);
	return (args);
}

/**
 * @brief 명령어 suffix 노드에 포함된 인자 수를 계산한다.
 *
 * NODE_CMD_SUFFIX 체인을 순회하여 NODE_WORD 타입의 인자 개수를 센다.
 *
 * @param cmd_suffix 명령어 suffix를 나타내는 AST 노드
 * @return int 인자의 개수
 */
static int	count_args(t_syntax_node *cmd_suffix)
{
	int				count;
	t_syntax_node	*cur_node;

	if (!cmd_suffix)
		return (0);
	cur_node = cmd_suffix;
	count = 0;
	while (cur_node->type == NODE_CMD_SUFFIX)
	{
		if (cur_node->value.b_node.left->type == NODE_WORD)
			count++;
		cur_node = cur_node->value.b_node.right;
	}
	if (cur_node->type == NODE_WORD)
		count++;
	return (count);
}

void	clear_cmd_form(t_cmd_form *cmd_form)
{
	char	**strs;

	if (!cmd_form)
		return ;
	if (cmd_form->cmd)
		free(cmd_form->cmd);
	cmd_form->cmd = NULL;
	strs = cmd_form->args;
	if (strs)
	{
		while (*strs)
			free(*strs++);
	}
	if (cmd_form->args)
		free(cmd_form->args);
	cmd_form->args = NULL;
	strs = cmd_form->envp;
	if (strs)
	{
		while (*strs)
			free(*strs++);
	}
	if (cmd_form->envp)
		free(cmd_form->envp);
	cmd_form->envp = NULL;
}

/**
 * @brief 명령 앞(prefix)에 붙은 I/O 리다이렉션 체인을 적용한다.
 *
 * NODE_CMD_PREFIX로 연결된 이진 트리를 좌측부터 순회하며
 * 각 노드의 I/O 리다이렉션을 eval_io_redir()로 적용한다.
 *
 * @param prefix 명령의 prefix 노드(또는 단일 I/O 노드)
 * @return t_status SUCCESS(0) 또는 ERROR(비0)
 *
 * @note
 * - prefix가 체인이면 left를 먼저 적용하고 right로 진행한다.
 * - 어느 한 단계라도 오류가 발생하면 즉시 해당 status를 반환한다.
 */
t_status	set_io_from_prefix(t_syntax_node *prefix)
{
	t_syntax_node	*cur_node;
	t_status		status;

	if (!prefix || prefix->eval == OFF)
		return (SUCCESS);
	cur_node = prefix;
	while (cur_node && cur_node->type == NODE_CMD_PREFIX)
	{
		status = eval_io_redir(cur_node->value.b_node.left);
		if (status)
			return (status);
		cur_node = cur_node->value.b_node.right;
	}
	status = eval_io_redir(cur_node);
	return (status);
}

/**
 * @brief 명령 뒤(suffix)에 붙은 I/O 리다이렉션/인자 체인을 적용한다.
 *
 * NODE_CMD_SUFFIX로 연결된 이진 트리를 좌측부터 순회하며
 * 각 노드의 I/O 리다이렉션을 eval_io_redir()로 적용한다.
 * (인자 수집은 별도 경로에서 수행되며, 여기서는 I/O만 처리한다고 가정)
 *
 * @param suffix 명령의 suffix 노드(또는 단일 I/O 노드)
 * @return t_status SUCCESS(0) 또는 ERROR(비0)
 *
 * @note
 * - suffix가 체인이면 left를 먼저 적용하고 right로 진행한다.
 * - 어느 한 단계라도 오류가 발생하면 즉시 해당 status를 반환한다.
 */
t_status	set_io_from_suffix(t_syntax_node *suffix)
{
	t_syntax_node	*cur_node;
	t_status		status;

	if (!suffix || suffix->eval == OFF)
		return (SUCCESS);
	cur_node = suffix;
	while (cur_node && cur_node->type == NODE_CMD_SUFFIX)
	{
		status = eval_io_redir(cur_node->value.b_node.left);
		if (status)
			return (status);
		cur_node = cur_node->value.b_node.right;
	}
	status = eval_io_redir(cur_node);
	return (status);
}

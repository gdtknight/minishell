/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command_io_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 23:31:21 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 23:31:21 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

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
t_status	eval_prefix_io(t_syntax_node *prefix)
{
	t_syntax_node	*cur_node;
	t_status		status;

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
t_status	eval_suffix_io(t_syntax_node *suffix)
{
	t_syntax_node	*cur_node;
	t_status		status;

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

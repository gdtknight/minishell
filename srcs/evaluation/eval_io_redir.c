/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:39:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 06:46:44 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"

/**
 * @brief I/O 리다이렉션 노드를 평가하여 입출력 스트림을 재설정한다.
 *
 * 노드 타입에 따라 입력(STDIN) 또는 출력(STDOUT) 리다이렉션을 설정한다.
 * - NODE_IO_REDIR_IN / NODE_IO_REDIR_HEREDOC → set_stdin()
 * - NODE_IO_REDIR_OUT / NODE_IO_REDIR_APPEND → set_stdout()
 *
 * @param io_redir_node I/O 리다이렉션을 나타내는 구문 노드
 * @return t_status SUCCESS(0) 또는 ERROR(비0)
 *
 * @note
 * - NULL 노드 입력 시 아무 동작 없이 SUCCESS 반환.
 * - 각 하위 함수는 dup2()를 사용하여 표준 입출력을 새로운 FD로 교체한다.
 */
t_status	eval_io_redir(t_syntax_node *io_redir_node)
{
	if (!io_redir_node || io_redir_node->eval == OFF)
		return (SUCCESS);
	if (io_redir_node->type == NODE_IO_REDIR_IN
		|| io_redir_node->type == NODE_IO_REDIR_HEREDOC)
		return (set_stdin(io_redir_node));
	else if (io_redir_node->type == NODE_IO_REDIR_OUT
		|| io_redir_node->type == NODE_IO_REDIR_APPEND)
		return (set_stdout(io_redir_node));
	return (SUCCESS);
}

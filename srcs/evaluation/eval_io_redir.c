/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:39:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:07:02 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"

/**
 * @brief Evaluates I/O redirection for a syntax node.
 *
 * Calls set_stdin or set_stdout based on the node type. Returns SUCCESS if
 * the node is not an I/O redirection.
 *
 * @param io_redir_node Pointer to the I/O redirection node.
 * @return t_status SUCCESS or FAILURE.
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

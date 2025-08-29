/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:39:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 01:08:36 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "shell.h"

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
	{
		(get_shell_data())->is_redir = TRUE;
		return (set_stdin(io_redir_node));
	}
	else if (io_redir_node->type == NODE_IO_REDIR_OUT
		|| io_redir_node->type == NODE_IO_REDIR_APPEND)
	{
		(get_shell_data())->is_redir = TRUE;
		return (set_stdout(io_redir_node));
	}
	return (SUCCESS);
}

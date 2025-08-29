/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 03:30:48 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 20:22:46 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>

#include "libft.h"

#include "ast.h"
#include "shell.h"
#include "eval.h"

/**
 * @brief Waits for a child process and updates shell status.
 *
 * Waits for the given child PID and sets the shell's last_status based on
 * exit or signal.
 *
 * @param child_pid PID of the child process to wait for.
 * @param status Pointer to the status variable.
 * @param options Options for waitpid.
 */
void	wait_child(pid_t child_pid, int *status, int options)
{
	get_shell_data()->has_child = TRUE;
	waitpid(child_pid, status, options);
	if (WIFEXITED(*status))
		(get_shell_data())->last_status = WEXITSTATUS(*status);
	if (WIFSIGNALED(*status))
	{
		(get_shell_data())->last_status = 128 + WTERMSIG(*status);
		if (WTERMSIG(*status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		else if (WTERMSIG(*status) == SIGINT)
			ft_putendl_fd("", STDERR_FILENO);
	}
	get_shell_data()->has_child = FALSE;
}

/**
 * @brief Finds the nearest simple command node from an I/O redirection node.
 *
 * Traverses up the parent chain to locate the NODE_SIMPLE_COMMAND node.
 *
 * @param io_redir_node Pointer to the I/O redirection node.
 * @return t_syntax_node* Pointer to the found command node, or NULL if
 * not found.
 */
t_syntax_node	*find_cmd_node(t_syntax_node *io_redir_node)
{
	t_syntax_node	*cur;

	cur = io_redir_node;
	while (cur->type != NODE_SIMPLE_COMMAND)
	{
		if (cur->type == NODE_SEMICOLON
			|| cur->type == NODE_AMPERSAND
			|| cur->type == NODE_AND_IF
			|| cur->type == NODE_OR_IF
			|| cur->type == NODE_PIPELINE
			|| cur->type == NODE_PIPELINE_ERR)
			return (NULL);
		cur = cur->parent;
	}
	return (cur);
}

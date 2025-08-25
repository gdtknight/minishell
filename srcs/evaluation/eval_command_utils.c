/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 22:17:29 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:12:20 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "ast.h"
#include "eval.h"

static int	count_args(t_syntax_node *cmd_suffix);

/**
 * @brief Builds an argument array from a command suffix node.
 *
 * Traverses the suffix nodes and collects all word arguments into a
 * dynamically allocated array.
 *
 * @param cmd_suffix Pointer to the command suffix node.
 * @return char** Array of argument strings, NULL-terminated.
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
 * @brief Counts the number of arguments in a command suffix node.
 *
 * Traverses the suffix nodes and counts all word arguments.
 *
 * @param cmd_suffix Pointer to the command suffix node.
 * @return int Number of arguments found.
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

/**
 * @brief Frees all memory in a command form structure.
 *
 * Frees the command string, argument array, and environment array in the
 * given t_cmd_form struct.
 *
 * @param cmd_form Pointer to the command form to clear.
 */
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
 * @brief Evaluates and sets I/O redirections from a prefix node.
 *
 * Traverses the prefix nodes and applies I/O redirections using
 * eval_io_redir().
 *
 * @param prefix Pointer to the command prefix node.
 * @return t_status SUCCESS on success, error code otherwise.
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
 * @brief Evaluates and sets I/O redirections from a suffix node.
 *
 * Traverses the suffix nodes and applies I/O redirections using
 * eval_io_redir().
 *
 * @param suffix Pointer to the command suffix node.
 * @return t_status SUCCESS on success, error code otherwise.
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

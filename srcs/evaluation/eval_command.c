/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:34:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 02:11:36 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft.h"

#include "ast.h"
#include "shell.h"
#include "execute.h"
#include "utils.h"
#include "eval.h"

static void		eval_simple_command(t_command *command);
static void		eval_command_from_child(t_syntax_node *cmd_node);
static void		set_cmd_form(t_command *command);

/**
 * @brief Evaluates a command node, handling forking and execution.
 *
 * Forks a child process for non-builtin commands, or executes directly for
 * builtins and compound commands.
 *
 * @param cmd_node Pointer to the command syntax node to evaluate.
 */
void	eval_command(t_syntax_node *cmd_node)
{
	int		status;
	pid_t	pid;

	if (!cmd_node || cmd_node->eval == OFF)
		return ;
	if ((!(get_shell_data())->in_pipe)
		&& !((cmd_node->type == NODE_SIMPLE_COMMAND)
			&& is_builtin(cmd_node->value.command.cmd_word->value.word)))
	{
		pid = fork();
		if (pid == 0)
			eval_command_from_child(cmd_node);
		wait_child(pid, &status, 0);
		return ;
	}
	if (cmd_node->type == NODE_COMPOUND_COMMAND)
	{
		eval(cmd_node->value.child);
		return ;
	}
	eval_simple_command(&(cmd_node->value.command));
}

/**
 * @brief Evaluates a command node in a child process.
 *
 * Handles both compound and simple commands, then clears shell state and
 * exits.
 *
 * @param cmd_node Pointer to the command syntax node to evaluate.
 */
static void	eval_command_from_child(t_syntax_node *cmd_node)
{
	if (cmd_node->type == NODE_COMPOUND_COMMAND)
	{
		eval(cmd_node->value.child);
		return ;
	}
	eval_simple_command(&(cmd_node->value.command));
	clear_heredoc_input();
	clear_shell_input();
	clear_shell_data();
	exit(get_shell_data()->last_status);
}

/**
 * @brief Evaluates a simple command node.
 *
 * Sets up the command form, handles I/O redirections, and executes the
 * command or builtin.
 *
 * @param command Pointer to the command structure to evaluate.
 */
static void	eval_simple_command(t_command *command)
{
	set_cmd_form(command);
	if (set_io_from_prefix(command->prefix) == ERROR
		|| set_io_from_suffix(command->suffix) == ERROR)
	{
		(get_shell_data())->last_status = EXIT_FAILURE;
		return ;
	}
	if (get_shell_input()->input_node->eval == OFF
		|| !(command->cmd_word))
		return ;
	if (is_builtin(command->cmd_word->value.word))
	{
		execute_builtin(&(command->form));
		return ;
	}
	execute_command(&(command->form));
}

/**
 * @brief Sets up the command form structure for execution.
 *
 * Fills in the command, arguments, and environment for the given command
 * structure.
 *
 * @param command Pointer to the command structure to set up.
 */
static void	set_cmd_form(t_command *command)
{
	command->form.cmd = NULL;
	command->form.args = NULL;
	command->form.envp = NULL;
	command->form.args = get_args_from_suffix(command->suffix);
	command->form.envp = lst_from_hashmap(&(get_shell_data()->envp_map));
	if (command->cmd_word)
	{
		command->form.cmd = ft_strdup(command->cmd_word->value.word);
		(command->form.args)[0] = \
			ft_strdup((char *)(command->cmd_word->value.word));
	}
	else
	{
		command->form.cmd = ft_strdup("");
		(command->form.args)[0] = ft_strdup("");
	}
}

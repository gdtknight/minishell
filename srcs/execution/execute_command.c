/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:25:06 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 22:00:00 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "shell.h"

#include "execute.h"

static void	execute_absolute_path(t_cmd_form *cmd_form);
static void	is_a_directory(void);
static void	execute_relative_path(t_cmd_form *cmd_form);

/**
 * @brief Executes a command using the command form structure.
 *
 * Handles relative and absolute paths, checks permissions, and calls execve.
 *
 * @param cmd_form Pointer to the command form structure.
 */
void	execute_command(t_cmd_form *cmd_form)
{
	if (!cmd_form || !(cmd_form->cmd))
		return ;
	execute_absolute_path(cmd_form);
	execute_relative_path(cmd_form);
}

static void	execute_absolute_path(t_cmd_form *cmd_form)
{
	char	*cmd;

	cmd = cmd_form->cmd;
	if ((*cmd == '.' && *(cmd + 1) == '/')
		|| *cmd == '/')
	{
		if (*(cmd + 2) == '\0')
			is_a_directory();
		if (access(cmd, R_OK) != 0)
		{
			perror(cmd);
			exit(COMMAND_NOT_FOUND_CODE);
		}
		if (access(cmd, X_OK) != 0)
		{
			perror(cmd);
			exit(PERMISSION_DENIED_CODE);
		}
		restore_signal();
		execve(cmd, cmd_form->args, cmd_form->envp);
	}
}

static void	is_a_directory(void)
{
	ft_putstr_fd("./", STDERR_FILENO);
	ft_putendl_fd(": Is a directory", STDERR_FILENO);
	clear_heredoc_input();
	clear_shell_input();
	clear_shell_data();
	exit(PERMISSION_DENIED_CODE);
}

/**
 * @brief Executes a command by searching for its absolute path.
 *
 * Finds the command in PATH, updates the command form, and calls execve.
 *
 * @param cmd_form Pointer to the command form structure.
 */
static void	execute_relative_path(t_cmd_form *cmd_form)
{
	char		*path;

	path = find_path(cmd_form->cmd, cmd_form->envp);
	if (path)
	{
		free(cmd_form->cmd);
		cmd_form->cmd = (char *)path;
		free(cmd_form->args[0]);
		cmd_form->args[0] = (char *)path;
		restore_signal();
		execve(path, cmd_form->args, cmd_form->envp);
	}
	clear_heredoc_input();
	clear_shell_input();
	clear_shell_data();
	exit(COMMAND_NOT_FOUND_CODE);
}

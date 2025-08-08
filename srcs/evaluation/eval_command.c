/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:34:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 20:11:26 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "def.h"
#include "libft.h"

#include "eval.h"
#include "envp.h"
#include "parser.h"
#include "shell_data.h"

static int		execute_simple_cmd(t_command *cmd);
static t_status	execute_cmd(t_cmd_form cmd_form);
static t_status	eval_prefix_io(t_syntax_node *prefix);
static t_status	eval_suffix_io(t_syntax_node *suffix);

t_status	eval_command(t_syntax_node *cmd_node)
{
	int		status;
	pid_t	pid;

	if ((get_shell_data())->in_pipe)
	{
		if (cmd_node->type == NODE_COMPOUND_COMMAND)
			exit(eval(cmd_node->value.child));
		else if (cmd_node->type == NODE_SIMPLE_COMMAND)
			exit(execute_simple_cmd(&(cmd_node->value.command)));
		else
			exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0)
	{
		if (cmd_node->type == NODE_COMPOUND_COMMAND)
			exit(eval(cmd_node->value.child));
		else if (cmd_node->type == NODE_SIMPLE_COMMAND)
			exit(execute_simple_cmd(&(cmd_node->value.command)));
		else
			exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	return (status);
}

static int	execute_simple_cmd(t_command *cmd)
{
	t_status	status;
	t_cmd_form	cmd_form;

	cmd_form.args = get_args_from_suffix(cmd->suffix);
	cmd_form.cmd = ft_strdup(cmd->word);
	(cmd_form.args)[0] = ft_strdup(cmd->word);
	cmd_form.envp = lst_from_hashmap(&(get_shell_data()->envp_map));
	status = 0;
	if (eval_prefix_io(cmd->prefix) == ERROR
		|| eval_suffix_io(cmd->suffix) == ERROR)
		return (ERROR);
	if (execute_cmd(cmd_form))
	{
		perror(strerror(errno));
		return (errno);
	}
	return (status);
}

/**
 * @brief 주어진 명령어를 실행합니다.
 * 
 * 실행할 명령어를 `tokenize`로 나누고, 해당 명령어가 실행 가능한지 확인합니다.
 * 실행 가능한 경로를 찾으면 해당 경로에서 명령어를 실행합니다.
 * 
 * @param argv 실행할 명령어
 * @param envp 환경 변수
 * @return 실행 실패 시 FALSE 반환
 */
static t_status	execute_cmd(t_cmd_form cmd_form)
{
	const char	*__path;

	if (access(cmd_form.cmd, X_OK) == 0)
		execve(cmd_form.cmd, cmd_form.args, cmd_form.envp);
	else
	{
		__path = find_path(cmd_form.cmd, cmd_form.envp);
		if (__path)
		{
			free(cmd_form.cmd);
			cmd_form.cmd = (char *)__path;
			free((cmd_form.args)[0]);
			(cmd_form.args)[0] = (char *)__path;
			execve(cmd_form.cmd, cmd_form.args, cmd_form.envp);
		}
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putendl_fd(cmd_form.cmd, 2);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(EXIT_FAILURE);
}

static t_status	eval_prefix_io(t_syntax_node *prefix)
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

static t_status	eval_suffix_io(t_syntax_node *suffix)
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

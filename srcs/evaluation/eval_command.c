/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:34:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:02:33 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "libft.h"

#include "eval.h"
#include "envp.h"
#include "parser.h"
#include "shell_data.h"

static int		execute(t_command *cmd);
static t_status	execute_cmd(t_cmd_form cmd_form);
static int		eval_prefix_io(t_syntax_node *prefix);
static int		eval_suffix_io(t_syntax_node *suffix);

int	eval_command(t_syntax_node *cmd_node)
{
	int		status;
	int		child_pid;

	if (cmd_node->type == NODE_COMPOUND_COMMAND)
	{
		child_pid = fork();
		if (child_pid == 0)
			exit(eval(cmd_node->value.child));
		waitpid(child_pid, &status, 0);
	}
	else
		status = execute(&(cmd_node->value.command));
	return (status);
}

int	execute(t_command *cmd)
{
	int			status;
	pid_t		child;
	t_cmd_form	cmd_form;

	cmd_form.args = get_args_from_suffix(cmd->suffix);
	cmd_form.cmd = ft_strdup(cmd->word);
	(cmd_form.args)[0] = ft_strdup(cmd->word);
	cmd_form.envp = lst_from_hashmap(&(get_shell_data()->envp_map));
	status = 0;
	child = fork();
	if (child == 0)
	{
		if (eval_prefix_io(cmd->prefix) > 0 || eval_suffix_io(cmd->suffix) > 0)
			return (1);
		if (execute_cmd(cmd_form))
		{
			perror(strerror(errno));
			return (errno);
		}
	}
	else
		waitpid(child, &status, 0);
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
t_status	execute_cmd(t_cmd_form cmd_form)
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
	exit(EXIT_FAILURE);
}

int	eval_prefix_io(t_syntax_node *prefix)
{
	t_syntax_node	*cur_node;
	int				status;

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

int	eval_suffix_io(t_syntax_node *suffix)
{
	t_syntax_node	*cur_node;
	int				status;

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

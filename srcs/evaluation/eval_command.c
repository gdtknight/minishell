/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:34:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 03:55:42 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft.h"

#include "ast.h"
#include "shell.h"
#include "execute.h"
#include "eval.h"
#include "utils.h"

#include "debug.h"

static void		eval_simple_command(t_command *command);
static void		set_cmd_form(t_command *command);
static t_status	set_io_from_prefix(t_syntax_node *prefix);
static t_status	set_io_from_suffix(t_syntax_node *suffix);

/**
 * @brief (파이프 여부에 따라) 명령 노드를 실행한다.
 *
 * - 파이프 내부(get_shell_data()->in_pipe == TRUE)라면 현재 프로세스에서
 *   즉시 하위 노드를 eval/execute_simple_cmd로 실행하고 exit()로 종료한다.
 * - 파이프 외부라면 fork() 후 자식에서 실행하고, 부모는 waitpid()로 대기한다.
 *
 * @param cmd_node 실행할 명령 노드 (NODE_SIMPLE_COMMAND 또는 NODE_COMPOUND_COMMAND)
 * @return t_status waitpid()로 수집한 자식의 상태 값(원시 status)
 *
 * @note
 * - 반환값은 WEXITSTATUS가 아닌 waitpid의 원시 status이다.
 *   호출 측에서 WIFEXITED/WEXITSTATUS로 해석이 필요할 수 있다.
 * - 자식에서는 restore_terminal_settings()를 호출한 뒤 실행한다.
 */
void	eval_command(t_syntax_node *cmd_node)
{
	int		status;
	pid_t	pid;

	if ((get_shell_data())->in_pipe)
	{
		if (cmd_node->type == NODE_COMPOUND_COMMAND)
		{
			eval(cmd_node->value.child);
			return ;
		}
		eval_simple_command(&(cmd_node->value.command));
		exit(get_shell_data()->last_status);
	}
	if ((cmd_node->type == NODE_SIMPLE_COMMAND
		&& is_builtin(cmd_node->value.command.word)))
	{
		eval_simple_command(&(cmd_node->value.command));
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (cmd_node->type == NODE_COMPOUND_COMMAND)
		{
			eval(cmd_node->value.child);
			return ;
		}
		eval_simple_command(&(cmd_node->value.command));
		exit(get_shell_data()->last_status);
	}
	wait_child(pid, &status, 0);
}

static void	eval_simple_command(t_command *command)
{
	set_cmd_form(command);
	if (set_io_from_prefix(command->prefix) == ERROR
		|| set_io_from_suffix(command->suffix) == ERROR)
	{
		(get_shell_data())->last_status = EXIT_FAILURE;
		return ;
	}
	if (is_builtin(command->word))
	{
		execute_builtin(&(command->form));
		return ;
	}
	execute_command(&(command->form));
}

static void	set_cmd_form(t_command *command)
{
	command->form.cmd = NULL;
	command->form.args = NULL;
	command->form.envp = NULL;
	command->form.args = get_args_from_suffix(command->suffix);
	command->form.cmd = ft_strdup(command->word);
	(command->form.args)[0] = ft_strdup((char *)(command->word));
	command->form.envp = lst_from_hashmap(&(get_shell_data()->envp_map));
}

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
static t_status	set_io_from_prefix(t_syntax_node *prefix)
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
static t_status	set_io_from_suffix(t_syntax_node *suffix)
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

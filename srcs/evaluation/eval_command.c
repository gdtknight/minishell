/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:34:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 21:54:41 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "def.h"
#include "libft.h"

#include "eval.h"
#include "parser.h"
#include "shell_data.h"
#include "utils.h"
#include "sig.h"

static t_status	execute_simple_cmd(t_command *cmd);
static t_status	execute_cmd(t_cmd_form *cmd_form);
static t_status	execute_absolute_path(t_cmd_form *cmd_form);

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
t_status	eval_command(t_syntax_node *cmd_node)
{
	int		status;
	pid_t	pid;

	if ((get_shell_data())->in_pipe)
	{
		if (cmd_node->type == NODE_SIMPLE_COMMAND)
			exit(execute_simple_cmd(&(cmd_node->value.command)));
		else
			exit(eval(cmd_node->value.child));
	}
	if (cmd_node->type == NODE_SIMPLE_COMMAND
		&& is_builtin(cmd_node->value.command.word))
		return (execute_simple_cmd(&(cmd_node->value.command)));
	pid = fork();
	if (pid == 0)
	{
		restore_signals();
		if (cmd_node->type == NODE_SIMPLE_COMMAND)
			exit(execute_simple_cmd(&(cmd_node->value.command)));
		else
			exit(eval(cmd_node->value.child));
	}
	return (wait_child(pid, &status, 0));
}

/**
 * @brief 단일(simple) 명령을 구성하고 실행한다.
 *
 * suffix에서 인자 벡터를 확보하고, word를 argv[0]로 설정한다.
 * prefix/suffix의 I/O 리다이렉션을 적용한 뒤, 빌트인 실행을 우선 시도하고
 * 실패 시 외부 프로그램 실행을 시도한다.
 *
 * @param cmd 파싱된 단일 명령 정보(선행 prefix/후행 suffix 포함)
 * @return t_status SUCCESS(0) 또는 ERROR(비0)
 *
 * @note
 * - execute_builtin()이 성공하면 외부 실행은 시도하지 않는다.
 * - 외부 실행은 execute_cmd()에서 처리하며, 해당 함수는 성공 시 execve로 교체되어
 *   복귀하지 않고, 실패 시 에러 메시지 출력 후 exit(EXIT_FAILURE)로 종료한다.
 * - eval_prefix_io()/eval_suffix_io() 중 하나라도 ERROR를 반환하면 ERROR를 즉시 반환한다.
 */
static t_status	execute_simple_cmd(t_command *cmd)
{
	t_cmd_form	cmd_form;

	cmd_form.args = get_args_from_suffix(cmd->suffix);
	cmd_form.cmd = ft_strdup(cmd->word);
	(cmd_form.args)[0] = ft_strdup((char *)(cmd->word));
	cmd_form.envp = lst_from_hashmap(&(get_shell_data()->envp_map));
	if (eval_prefix_io(cmd->prefix) == ERROR
		|| eval_suffix_io(cmd->suffix) == ERROR)
		return (ERROR);
	if (execute_builtin(cmd_form) == FAILURE)
		execute_cmd(&cmd_form);
	return (SUCCESS);
}

/**
 * @brief 외부 프로그램을 execve로 실행한다(성공 시 반환하지 않음).
 *
 * - cmd_form.cmd가 실행 가능(X_OK)이면 해당 경로로 execve().
 * - 아니면 PATH 탐색(find_path)로 실행 경로를 찾아 execve().
 * - 모든 경로가 실패하면 "command not found"를 stderr로 출력하고
 *   표준입출력을 닫은 뒤 exit(EXIT_FAILURE).
 *
 * @param cmd_form 실행할 명령의 경로/인자/환경(소유권은 호출자에게 있음)
 * @return t_status 이 함수는 정상 경로에서 반환하지 않으며, 실패 시 exit()한다.
 *
 * @note
 * - 성공 시 프로세스 이미지가 교체되어 제어가 돌아오지 않는다.
 * - 실패 루트에서만 오류 메시지 출력 후 프로세스를 종료한다.
 * - argv[0]은 실제 실행 경로로 갱신된다.
 */
static t_status	execute_cmd(t_cmd_form *cmd_form)
{
	char		*cmd;

	cmd = cmd_form->cmd;
	if (*cmd == '.' && *(cmd + 1) == '/')
	{
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
		execve(cmd, cmd_form->args, cmd_form->envp);
	}
	return (execute_absolute_path(cmd_form));
}

static t_status	execute_absolute_path(t_cmd_form *cmd_form)
{
	char	*path;

	path = find_path(cmd_form->cmd, cmd_form->envp);
	if (path)
	{
		free(cmd_form->cmd);
		cmd_form->cmd = (char *)path;
		free(cmd_form->args[0]);
		cmd_form->args[0] = (char *)path;
		execve(path, cmd_form->args, cmd_form->envp);
	}
	exit(COMMAND_NOT_FOUND_CODE);
}

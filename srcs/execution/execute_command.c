/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:25:06 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 06:13:59 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "eval.h"
#include "shell_data.h"

#include "execute.h"

#include "debug.h"

static void	execute_absolute_path(t_cmd_form *cmd_form);

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
void	execute_command(t_cmd_form *cmd_form)
{
	char		*cmd;

	cmd = cmd_form->cmd;
	if (*cmd == '.' && *(cmd + 1) == '/')
	{
		if (access(cmd, R_OK) != 0)
		{
			perror(cmd);
			clear_shell_input();
			clear_shell_data();
			exit(COMMAND_NOT_FOUND_CODE);
		}
		if (access(cmd, X_OK) != 0)
		{
			perror(cmd);
			clear_shell_input();
			clear_shell_data();
			exit(PERMISSION_DENIED_CODE);
		}
		debug("[execute_cmd] pid : %d - cmd : %s", getpid(), cmd_form->cmd);
		restore_signal();
		execve(cmd, cmd_form->args, cmd_form->envp);
	}
	execute_absolute_path(cmd_form);
}

static void	execute_absolute_path(t_cmd_form *cmd_form)
{
	char	*path;

	path = find_path(cmd_form->cmd, cmd_form->envp);
	if (path)
	{
		free(cmd_form->cmd);
		cmd_form->cmd = (char *)path;
		free(cmd_form->args[0]);
		cmd_form->args[0] = (char *)path;
		debug("[execute_absolute_path] pid : %d - cmd : %s", getpid(), cmd_form->cmd);
		restore_signal();
		execve(path, cmd_form->args, cmd_form->envp);
	}
	clear_shell_input();
	clear_shell_data();
	exit(COMMAND_NOT_FOUND_CODE);
}

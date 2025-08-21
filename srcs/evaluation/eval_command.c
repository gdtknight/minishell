/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:34:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 12:47:05 by yoshin           ###   ########.fr       */
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

	if (!cmd_node || cmd_node->eval == OFF)
		return ;
	if ((!(get_shell_data())->in_pipe)
		&& !((cmd_node->type == NODE_SIMPLE_COMMAND)
			&& is_builtin(cmd_node->value.command.word)))
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

static void	eval_simple_command(t_command *command)
{
	set_cmd_form(command);
	if (set_io_from_prefix(command->prefix) == ERROR
		|| set_io_from_suffix(command->suffix) == ERROR)
	{
		(get_shell_data())->last_status = EXIT_FAILURE;
		return ;
	}
	if (get_shell_input()->input_node->eval == OFF)
		return ;
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

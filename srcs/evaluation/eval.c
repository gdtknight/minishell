/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:35:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 08:40:17 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

#include "def.h"
#include "ast.h"
#include "shell.h"

#include "eval.h"

/**
 * @brief 구문 트리 노드를 평가하여 명령을 실행한다.
 *
 * AST 노드의 타입에 따라 적절한 평가 함수를 호출하여 명령을 실행한다.
 * - NODE_SEMICOLON / NODE_AMPERSAND: eval_list()로 처리
 * - NODE_AND_IF / NODE_OR_IF: eval_and_or()로 처리
 * - NODE_PIPELINE / NODE_PIPELINE_ERR: eval_pipeline()로 처리 (파이프 플래그 on/off)
 * - NODE_SIMPLE_COMMAND / NODE_COMPOUND_COMMAND: eval_command()로 처리
 *
 * @param node 평가할 구문 트리 노드
 * @return t_status 명령 실행 결과 상태 코드
 *
 * @note
 * - 현재 NODE_AMPERSAND(백그라운드 실행)는 지원하지 않으며, NODE_SEMICOLON과 동일하게 동기 처리된다.
 * - eval_pipeline() 실행 시 get_shell_data()->in_pipe를 TRUE로 설정해 파이프 상태를 알린다.
 */
void	eval(t_syntax_node *node)
{
	if (!node || node->eval == OFF)
		return ;
	if (node->type == NODE_SEMICOLON || node->type == NODE_AMPERSAND)
	{
		eval_list(node);
		return ;
	}
	if (node->type == NODE_AND_IF || node->type == NODE_OR_IF)
	{
		eval_and_or(node);
		return ;
	}
	if (node->type == NODE_PIPELINE || node->type == NODE_PIPELINE_ERR)
	{
		get_shell_data()->in_pipe = TRUE;
		eval_pipeline(node);
		get_shell_data()->in_pipe = FALSE;
		return ;
	}
	if (node->type == NODE_SIMPLE_COMMAND || node->type == NODE_COMPOUND_COMMAND)
	{
		eval_command(node);
		return ;
	}
	(get_shell_data())->last_status = EXIT_FAILURE;
}

/**
 * @brief 명령 리스트 노드(NODE_SEMICOLON, NODE_AMPERSAND)를 평가한다.
 *
 * 리스트의 왼쪽 명령을 먼저 실행한 뒤, 오른쪽 명령을 실행한다.
 * 현재는 & 기호(백그라운드 실행)를 지원하지 않으며, 모든 명령을 동기적으로 처리한다.
 *
 * @param list_node 명령 리스트를 나타내는 AST 노드
 * @return t_status 마지막 명령 실행 결과 상태 코드
 *
 * @note
 * - 왼쪽 명령은 fork()로 자식 프로세스에서 실행 후 waitpid()로 대기한다.
 * - 오른쪽 명령은 부모 프로세스에서 바로 eval()로 실행한다.
 */
void	eval_list(t_syntax_node *node)
{
	int			status;
	pid_t		child_pid;

	if (!node || node->eval == OFF)
		return ;
	if (node->type == NODE_SEMICOLON)
	{
		child_pid = fork();
		if (child_pid == 0)
		{
			eval(node->value.b_node.left);
			return ;
		}
		waitpid(child_pid, &status, 0);
		eval(node->value.b_node.right);
	}
	// & 의 경우 pthread 생성해서 백그라운드 실행하고 추가 처리 필요함
	else
	{
		child_pid = fork();
		if (child_pid == 0)
		{
			eval(node->value.b_node.left);
			return ;
		}
		waitpid(child_pid, &status, WNOHANG);
		eval(node->value.b_node.right);
	}
}

/**
 * @brief AND/OR 연산자 노드를 평가한다.
 *
 * 왼쪽 명령을 먼저 실행한 뒤, 결과에 따라 오른쪽 명령 실행 여부를 결정한다.
 * - NODE_AND_IF: 왼쪽 명령이 성공(status == 0)하면 오른쪽 명령 실행
 * - NODE_OR_IF:  왼쪽 명령이 실패(status != 0)하면 오른쪽 명령 실행
 *
 * @param and_or_node AND/OR 연산자를 나타내는 AST 노드
 * @return t_status 마지막으로 실행된 명령의 상태 코드
 */
void	eval_and_or(t_syntax_node *and_or_node)
{
	if (!and_or_node || and_or_node->eval == OFF)
		return ;
	eval(and_or_node->value.b_node.left);
	if (and_or_node->type == NODE_AND_IF
		&& (get_shell_data())->last_status == SUCCESS)
		eval(and_or_node->value.b_node.right);
	else if (and_or_node->type == NODE_OR_IF
		&& (get_shell_data())->last_status != SUCCESS)
		eval(and_or_node->value.b_node.right);
}

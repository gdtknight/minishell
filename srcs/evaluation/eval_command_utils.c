/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 22:17:29 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 10:15:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "ast.h"

static int	count_args(t_syntax_node *cmd_suffix);

/**
 * @brief 명령어 suffix 노드에서 인자(argv) 배열을 생성한다.
 *
 * NODE_CMD_SUFFIX로 연결된 트리를 순회하며 NODE_WORD 타입의 값을
 * 인자로 추가하고, 마지막에 NULL 포인터로 배열을 종료한다.
 * argv[0]은 호출부에서 명령어로 설정한다.
 *
 * @param cmd_suffix 명령어 suffix를 나타내는 AST 노드
 * @return char** NULL 종료된 인자 문자열 배열(동적 할당)
 *
 * @note
 * - count_args()를 이용해 배열 크기를 미리 계산하여 메모리 할당.
 * - 반환된 배열과 내부 문자열은 호출자가 free()로 해제해야 한다.
 * - prefix의 인자는 포함하지 않는다.
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
 * @brief 명령어 suffix 노드에 포함된 인자 수를 계산한다.
 *
 * NODE_CMD_SUFFIX 체인을 순회하여 NODE_WORD 타입의 인자 개수를 센다.
 *
 * @param cmd_suffix 명령어 suffix를 나타내는 AST 노드
 * @return int 인자의 개수
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

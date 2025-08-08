/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:33:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:00:12 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "eval.h"

static int	count_args(t_syntax_node *cmd_suffix);
static void	release_dirs(char ***p_dirs);

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
 * @brief 주어진 명령어의 경로를 찾습니다.
 * 
 * 환경 변수인 PATH에 설정된 디렉토리들을 순차적으로 확인하여
 * 명령어를 실행할 수 있는 경로를 찾습니다.
 * 
 * @param cmd 실행할 명령어
 * @param envp 환경 변수
 * @return 실행 가능한 경로가 있을 경우 해당 경로, 없으면 NULL 반환
 */
char	*find_path(char *cmd, char *envp[])
{
	char	*path_temp;
	char	*path_full;
	char	**dir;
	char	**dirs;

	dirs = NULL;
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp++, 5) == 0)
			dirs = ft_split((*(envp - 1) + 5), ':');
	}
	dir = dirs;
	while (*dir)
	{
		path_temp = ft_strjoin(*dir++, "/");
		path_full = ft_strjoin(path_temp, cmd);
		free(path_temp);
		if (access(path_full, X_OK) == 0)
			break ;
		free(path_full);
	}
	release_dirs(&dirs);
	return (path_full);
}

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

/**
 * @brief 디렉토리 배열을 해제합니다.
 * 
 * `find_path` 함수에서 사용하는 디렉토리 배열을 해제합니다.
 * 
 * @param p_dirs 디렉토리 배열의 포인터
 */
static void	release_dirs(char ***p_dirs)
{
	char	**path;

	if (!p_dirs || !*p_dirs)
		return ;
	path = *p_dirs;
	while (*path)
		free(*path++);
	free(*p_dirs);
	*p_dirs = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 23:04:02 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 03:52:25 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include "libft.h"

#include "def.h"
#include "eval.h"

#include "execute.h"

static char			**get_dirs(char *envp[]);
static t_boolean	check_path(char *path);
static void			release_dirs(char ***p_dirs);

/**
 * @brief 실행 가능한 명령어의 전체 경로를 PATH 환경변수에서 찾는다.
 *
 * PATH 환경변수에 지정된 디렉토리를 순회하며, cmd와 결합한 경로가
 * 실행 가능(X_OK)인지 확인한다. 실행 가능한 경로를 찾으면 해당 경로를 반환한다.
 *
 * @param cmd  실행할 명령어(파일명)
 * @param envp 환경 변수 배열(NULL 종료)
 * @return char* 실행 가능한 전체 경로(동적 할당) 또는 NULL(없을 경우)
 *
 * @note
 * - PATH 변수는 ':'로 구분된 디렉토리 목록을 ft_split()으로 분리하여 탐색.
 * - 첫 번째 실행 가능한 경로를 찾으면 즉시 반환한다.
 * - 반환된 문자열은 호출자가 free()로 해제해야 한다.
 * - dirs 배열은 release_dirs()로 해제한다.
 */
char	*find_path(char *cmd, char *envp[])
{
	char	*path_temp;
	char	*path_full;
	char	**dir;
	char	**dirs;

	dirs = get_dirs(envp);
	dir = dirs;
	while (*dir)
	{
		path_temp = ft_strjoin(*dir++, "/");
		path_full = ft_strjoin(path_temp, cmd);
		free(path_temp);
		if (check_path(path_full))
			break ;
		free(path_full);
		path_full = NULL;
	}
	release_dirs(&dirs);
	if (!path_full)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	return (path_full);
}

static t_boolean	check_path(char *path)
{
	if (access(path, R_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (TRUE);
		else
		{
			perror(path);
			free(path);
			return (FALSE);
		}
	}
	return (FALSE);
}

static char	**get_dirs(char *envp[])
{
	char	**dirs;

	while (*envp)
		if (ft_strncmp("PATH=", *envp++, 5) == 0)
			dirs = ft_split((*(envp - 1) + 5), ':');
	return (dirs);
}

/**
 * @brief find_path()에서 사용한 디렉토리 배열을 해제한다.
 *
 * ft_split()으로 생성된 디렉토리 배열을 모두 free()한 뒤
 * 포인터를 NULL로 초기화한다.
 *
 * @param p_dirs 해제할 디렉토리 배열의 포인터
 *
 * @note
 * - NULL 포인터나 비어 있는 배열에 대해서는 아무 동작도 하지 않는다.
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

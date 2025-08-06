/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:09 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/05 21:45:54 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "def.h"
#include "hashmap.h"

#include "builtin.h"


t_boolean	check_args(char *path)
{
	int			i;
	int			num;
	t_toggle	toggle;

	if (!path)
		return (TRUE);
	num = 0;
	i = 0;
	toggle = OFF;
	while (path[i])
	{
		if ((path[i] == '\'' || path[i] == '"') && toggle == OFF)
			toggle = ON;
		if ((path[i] == '\'' || path[i] == '"') && toggle == ON)
			toggle = OFF;
		if (path[i] == ' ' && toggle == OFF)
			num++;
		i++;
	}
	if (num > 1)
		return (FALSE);
	return (TRUE);
}


t_status	error_print(char *path, char *old_pwd)
{
	if (!old_pwd)
		printf("bash: cd: too many arguments\n");
	else if (!path)
		printf("bash: cd: HOME not set\n");
	else if (path[0] == '-')
		printf("cd: OLDPWD not set\n");
	else if (errno == EACCES)
		printf("bash: cd: permission denied: %s\n", path);
	else if (errno == ENOENT)
		printf("bash: cd: no such file or directory: %s\n", path);
	else if (errno == ENOTDIR)
		printf("bash: cd: not a directory: %s\n", path);
	if (old_pwd)
		free(old_pwd);
	return (FAILURE);
}

char	*route_set(char *path, char *old_pwd, t_hash_map *map)
{
	char	*new_pwd;

	if (!path || (path[0] == '-' && path[1] == '-' && path[2] == 0))
		new_pwd = get_value(map, "HOME");
	else if (path[0] == 0)
		new_pwd = old_pwd;
	else if (path[0] == '-' && path[1] == 0)
	{
		new_pwd = get_value(map, "OLDPWD");
		printf ("%s\n", new_pwd);
	}
	else
		new_pwd = path;
	return (new_pwd);
}

/**
 * @brief 현재 작업 디렉토리를 변경한다.
 *
 * 전달받은 경로로 작업 디렉토리를 변경하며, 변경 전의 디렉토리는 OLDPWD에,
 * 변경 후의 디렉토리는 PWD에 저장한다. 경로가 없으면 HOME 디렉토리로 이동한다.
 *
 * @param path  이동할 디렉토리 경로 (NULL일 경우 HOME 사용)
 * @param map   환경 변수가 저장된 해시맵
 * @return SUCCESS(변경 성공), FAIL(잘못된 인자 또는 접근 불가한 경로)
 */
t_status	builtin_cd(char *path, t_hash_map *map)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*cwd;

	if (!check_args(path))
		return (error_print(path, NULL));
	old_pwd = getcwd(NULL, 0);
	new_pwd = route_set(path, old_pwd, map);
	if (!new_pwd || chdir(new_pwd) != 0)
		return (error_print(path, old_pwd));
	if (old_pwd)
		put_key_value(map, "OLDPWD", old_pwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
		put_key_value(map, "PWD", cwd);
	else
		put_key_value(map, "PWD", ".");
	if (cwd)
		free (cwd);
	if (old_pwd)
		free (old_pwd);
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:09 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/22 16:49:36 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "def.h"

#include "hashmap.h"
#include "shell.h"
#include "builtin.h"

/**
 * @brief Count the number of arguments in a NULL-terminated string array.
 *
 * @param argc The argument array.
 * @return The number of arguments.
 */
int	count_argc(char **argc)
{
	int	i;

	i = 0;
	while (argc[i])
		i++;
	return (i);
}

/**
 * @brief Check for errors in cd arguments and set error type.
 *
 * @param argc The argument array.
 * @return The error type (t_cd_err).
 */
t_cd_err	check_error(char **argc)
{
	t_cd_err	err;
	int			i;

	i = 0;
	while (argc[i])
		i++;
	if (i > 2)
		err = CD_TOO_MANY_ARGS;
	else if (!argc[1] || (argc[1][0] == '-' && argc[1][1] == '-' &&
			argc[1][2] == 0))
		err = CD_HOME_NOT_SET;
	else if (argc[1][0] == '-')
		err = CD_OLDPWD_NOT_SET;
	else if (errno == EACCES)
		err = CD_EACCES;
	else if (errno == ENOENT)
		err = CD_ENOENT;
	else if (errno == ENOTDIR)
		err = CD_ENOTDIR;
	else
		err = CD_UNKNOWN;
	return (err);
}

/**
 * @brief Handle cd failure, print error messages, and set last status.
 *
 * @param err The error type.
 * @param old_pwd The previous working directory (to be freed if not NULL).
 * @return ERROR status.
 */
t_status	cd_fail(t_cd_err err, char *old_pwd)
{
	if (err == CD_TOO_MANY_ARGS)
	{
		get_shell_data()->last_status = 2;
		printf("bash: cd: too many arguments\n");
	}
	else
		get_shell_data()->last_status = 1;
	if (err == CD_HOME_NOT_SET)
		printf("bash: cd: HOME not set\n");
	if (err == CD_OLDPWD_NOT_SET)
		printf("cd: OLDPWD not set\n");
	if (err == CD_EACCES)
		perror("cd");
	if (err == CD_ENOENT)
		perror("cd");
	if (err == CD_ENOTDIR)
		perror("cd");
	if (old_pwd)
		free(old_pwd);
	return (ERROR);
}

/**
 * @brief Determine the target directory for cd based on arguments
 * and environment.
 * @param path The path argument from cd.
 * @param old_pwd The previous working directory.
 * @param map The environment variable hash map.
 * @return The resolved path to change directory to.
 */
char	*route_set(char *path, char *old_pwd, t_hash_map *map)
{
	char	*new_pwd;

	if (!path || (path[0] == '-' && path[1] == '-' && path[2] == 0))
		new_pwd = get_value(map, "HOME");
	else if (path[0] == 0)
		new_pwd = old_pwd;
	else if (path[0] == '-' && path[1] == 0)
	{
		if (get_value(map, "OLDPWD"))
		{
			new_pwd = get_value(map, "OLDPWD");
			printf ("%s\n", new_pwd);
		}
		else
			new_pwd = NULL;
	}
	else
		new_pwd = path;
	return (new_pwd);
}

/**
 * @brief Implementation of the builtin cd command.
 *
 * @param argc The argument array.
 * @return SUCCESS or ERROR status.
 */
t_status	builtin_cd(char **argc)
{
	t_hash_map	*envp_map;
	char		*old_pwd;
	char		*new_pwd;
	char		*cwd;

	envp_map = &get_shell_data()->envp_map;
	old_pwd = getcwd(NULL, 0);
	new_pwd = route_set(argc[1], old_pwd, envp_map);
	if (count_argc(argc) > 2 || !new_pwd || chdir(new_pwd) != 0)
		return (cd_fail(check_error(argc), old_pwd));
	if (old_pwd)
		put_key_value(envp_map, "OLDPWD", old_pwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
		put_key_value(envp_map, "PWD", cwd);
	else
		put_key_value(envp_map, "PWD", ".");
	if (cwd)
		free (cwd);
	if (old_pwd)
		free (old_pwd);
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}

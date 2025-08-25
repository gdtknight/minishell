/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 23:04:02 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:10:21 by jyoo             ###   ########.fr       */
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
 * @brief Finds the full path of a command using the PATH environment variable.
 *
 * Searches each directory in PATH for an executable matching the command.
 *
 * @param cmd Command name to search for.
 * @param envp Environment variable array.
 * @return char* Full path to the command, or NULL if not found.
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

/**
 * @brief Checks if a path is readable and executable.
 *
 * Returns TRUE if the file exists and is executable, FALSE otherwise.
 *
 * @param path Path to check.
 * @return t_boolean TRUE if executable, FALSE otherwise.
 */
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

/**
 * @brief Gets the list of directories from the PATH environment variable.
 *
 * Splits the PATH variable into an array of directory strings.
 *
 * @param envp Environment variable array.
 * @return char** Array of directory strings.
 */
static char	**get_dirs(char *envp[])
{
	char	**dirs;

	while (*envp)
		if (ft_strncmp("PATH=", *envp++, 5) == 0)
			dirs = ft_split((*(envp - 1) + 5), ':');
	return (dirs);
}

/**
 * @brief Frees the memory allocated for the directory array.
 *
 * Releases all memory used by the array of directory strings.
 *
 * @param p_dirs Pointer to the directory array to free.
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

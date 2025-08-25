/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:33:51 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:36:11 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expand_tilde_utils.c
 * @brief Utility functions for tilde (~) expansion.
 *
 * This module provides functions to construct and validate home directory paths
 * for tilde expansions in shell input.
 */

#include <stdlib.h>
#include <unistd.h>

#include "expand.h"

/**
 * @brief Get the home directory path for a given username.
 *
 * Constructs the full home directory path by appending the username to the
 * common home prefix (usually "/home/").
 *
 * @param username Dynamically allocated username string (will be freed)
 * @return char* Newly allocated full path to user's home directory
 */
char	*get_homedir(char *username)
{
	char	*home_dir;
	char	*home_prefix;

	home_prefix = get_home_prefix();
	home_dir = ft_strjoin(home_prefix, username);
	free(username);
	free(home_prefix);
	return (home_dir);
}

/**
 * @brief Check whether a given username has a readable home directory.
 *
 * @param username Username string to check
 * @return t_boolean TRUE if the home directory exists and is readable,
 *         FALSE otherwise
 */
t_boolean	check_homedir(char *username)
{
	t_boolean	result;
	char		*home_prefix;
	char		*home_dir;

	result = FALSE;
	home_prefix = get_home_prefix();
	home_dir = ft_strjoin(home_prefix, username);
	if (access(home_dir, R_OK) == 0)
		result = TRUE;
	free(home_dir);
	free(home_prefix);
	return (result);
}

/**
 * @brief Get the common home prefix path.
 *
 * Returns the part of the HOME environment variable up to the last slash.
 * For example, if HOME="/home/user", it returns "/home/".
 *
 * @return char* Newly allocated string of the home prefix
 */
char	*get_home_prefix(void)
{
	char	*home_path;
	char	*last_slash;

	home_path = getenv("HOME");
	last_slash = ft_strrchr(home_path, '/');
	return (ft_substr(
			home_path,
			0,
			ft_strlen(home_path) - ft_strlen(last_slash) + 1));
}

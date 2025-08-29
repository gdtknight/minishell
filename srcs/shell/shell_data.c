/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:06:19 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 20:19:35 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file shell_data.c
 * @brief Shell global data management functions.
 *
 * This file provides access to a global shell_data structure,
 * initializes it with environment variables, and clears its resources.
 */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"
#include "utils.h"
#include "hashmap.h"

#include "shell.h"

/**
 * @brief Returns a globally accessible shell_data structure.
 *
 * Maintains a static t_shell_data instance shared across the program
 * and returns its address.
 *
 * @return t_shell_data* Pointer to the global shell_data structure.
 *
 * @note
 * - The static variable persists for the lifetime of the program.
 * - Provides a safe way to share global state without exposing a 
 *   global variable.
 */
t_shell_data	*get_shell_data(void)
{
	static t_shell_data	shell_data;

	return (&shell_data);
}

/**
 * @brief Initializes the shell_data structure.
 *
 * Stores environment variables (envp) into a hashmap, backs up terminal
 * attributes, duplicates standard input/output file descriptors, and
 * sets initial state flags.
 *
 * @param envp Environment variable array passed to the program
 *             (NULL-terminated).
 * @return t_result COMPLETED on success.
 *
 * @note
 * - Each environment variable is split into key and value using extract_key() 
 *   and extract_value(), then stored via put_key_value().
 * - termios_backup stores the current terminal attributes.
 * - last_status is initialized to EXIT_SUCCESS (0).
 * - in_pipe flag is set to FALSE.
 * - stdin_fd and stdout_fd store duplicated standard input/output
 *   file descriptors.
 */
t_result	init_shell_data(char *envp[])
{
	t_hash_map	*map;
	char		*key;
	char		*value;

	map = &((get_shell_data())->envp_map);
	while (*envp != NULL)
	{
		key = extract_key(*envp);
		value = extract_value(*envp);
		put_key_value(map, key, value);
		free(key);
		free(value);
		envp++;
	}
	(get_shell_data())->last_status = EXIT_SUCCESS;
	(get_shell_data())->has_child = FALSE;
	(get_shell_data())->is_child = FALSE;
	(get_shell_data())->in_heredoc = FALSE;
	(get_shell_data())->last_bg_pid = -1;
	(get_shell_data())->last_arg = NULL;
	return (COMPLETED);
}

/**
 * @brief Releases resources inside shell_data.
 *
 * Clears the environment variable hashmap stored in shell_data
 * to prevent memory leaks.
 *
 * @note
 * - Frees last_arg if it was allocated.
 * - Clears all entries in envp_map.
 */
void	clear_shell_data(void)
{
	if (get_shell_data()->last_arg)
		free(get_shell_data()->last_arg);
	get_shell_data()->last_arg = NULL;
	clear_hashmap(&((get_shell_data())->envp_map));
}

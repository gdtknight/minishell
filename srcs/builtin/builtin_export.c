/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/29 18:19:29 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "hashmap.h"
#include "def.h"
#include "libft.h"
#include "builtin.h"
#include "shell.h"
#include "utils.h"

/**
 * @brief Prints an error message for invalid export identifiers.
 *
 * Outputs an error message to standard error indicating that the given key
 * is not a valid identifier for the export builtin.
 *
 * @param key The invalid identifier string.
 */
static void	print_error(char *key)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/**
 * @brief Checks if the given name is a valid environment variable identifier.
 *
 * @param name The name string to check.
 * @return TRUE if valid, FALSE otherwise.
 */
t_boolean	name_checker(char *name)
{
	int	i;

	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (FALSE);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * @brief Checks and sets an environment variable in the hash map if valid.
 *
 * @param envp The environment variable string (key=value).
 * @param map The environment variable hash map.
 * @return SUCCESS if set or valid, ERROR if invalid identifier.
 */
t_status	check_and_set_enp(char *envp, t_hash_map *map)
{
	char	*key;
	char	*value;
	int		status;

	status = SUCCESS;
	key = extract_key(envp);
	value = extract_value(envp);
	if (name_checker(key))
	{
		if (ft_strnchr(envp, '=', ft_strlen(envp)) == -1)
		{
			free (key);
			free (value);
			return (status);
		}
		put_key_value(map, key, value);
	}
	else
	{
		print_error(key);
		status = ERROR;
	}
	free (key);
	free (value);
	return (status);
}

/**
 * @brief Implementation of the builtin export command.
 *
 * @param argc The argument array.
 * @return SUCCESS or ERROR status after processing export arguments.
 */
t_status	builtin_export(char **argc)
{
	t_hash_map	*envp_map;
	t_status	status;
	int			i;

	envp_map = &get_shell_data()->envp_map;
	status = SUCCESS;
	get_shell_data()->last_status = 0;
	i = 1;
	while (argc[i])
	{
		if (check_and_set_enp(argc[i], envp_map) == ERROR)
		{
			status = ERROR;
			get_shell_data()->last_status = 1;
		}
		i++;
	}
	return (status);
}

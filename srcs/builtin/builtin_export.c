/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/30 09:10:10 by jyoo             ###   ########.fr       */
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

void	add_key_value(t_hash_map *map, char *key, char *value)
{
	t_hash_entry	*entry;
	char			*new_name;
	char			*temp;

	new_name = ft_substr(key, 0, ft_strlen(key) - 1);
	entry = get_entry(map, new_name);
	if (entry)
	{
		temp = ft_strjoin(entry->value, value);
		free (entry->value);
		entry->value = temp;
		free(new_name);
		return ;
	}
	entry = create_new_entry(new_name, value);
	put_entry(map, entry);
	free(new_name);
}

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
t_builtin_export	name_checker(char *name)
{
	int					i;
	t_builtin_export	status;

	status = NAME_VALID;
	if (!ft_isalpha(name[0]) && name[0] != '_')
		status = NAME_INVALID;
	if (name[1])
	{
		i = 1;
		while ((i < (int)ft_strlen(name) - 1) && status == 1)
		{
			if (!ft_isalnum(name[i]) && name[i] != '_')
				status = NAME_INVALID;
			i++;
		}
		if (name[i] != '+' && name[i] != '_' && !ft_isalnum(name[i]))
			status = NAME_VALID;
		if (name[i] == '+' && status == 1)
			status = NAME_APPEND;
	}
	return (status);
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
	if (name_checker(key) == NAME_VALID)
		put_key_value(map, key, value);
	else if (name_checker(key) == NAME_APPEND)
		add_key_value(map, key, value);
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

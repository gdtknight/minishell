/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/29 18:20:53 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashmap.h"
#include "shell.h"

#include "builtin.h"

/**
 * @brief Implementation of the builtin unset command.
 *
 * Removes environment variables specified in the argument
 * array from the shell's environment.
 *
 * @param argc The argument array.
 * @return SUCCESS after removing specified variables.
 */
t_status	builtin_unset(char **argc)
{
	t_hash_map	*envp_map;
	int			i;

	envp_map = &get_shell_data()->envp_map;
	get_shell_data()->last_status = 0;
	if (!argc)
		return (SUCCESS);
	i = 1;
	while (argc[i])
	{
		remove_entry(envp_map, get_entry(envp_map, argc[i]));
		i++;
	}
	return (SUCCESS);
}

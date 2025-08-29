/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/29 18:22:56 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "def.h"
#include "hashmap.h"
#include "builtin.h"
#include "shell.h"
#include "libft.h"

/**
 * @brief Implementation of the builtin env command.
 *
 * Prints all environment variables in the shell's environment.
 * @param argc The argument array (unused).
 * @return SUCCESS status after printing environment variables.
 */
t_status	builtin_env(char **argc)
{
	t_hash_map		map;
	t_hash_entry	*current;
	int				i;

	(void) argc;
	map = get_shell_data()->envp_map;
	i = 0;
	while (i < DEFAULT_HASHTABLE_SIZE)
	{
		current = map.table[i];
		while (current)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			current = current->next;
		}
		i++;
	}
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}

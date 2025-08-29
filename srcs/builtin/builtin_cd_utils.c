/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 03:00:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 03:05:06 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

void	update_pwd(void)
{
	char		*cwd;
	t_hash_map	*envp_map;

	envp_map = &((get_shell_data())->envp_map);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	if (cwd)
		put_key_value(envp_map, "PWD", cwd);
	else
		perror("cwd");
	if (cwd)
		free (cwd);
}

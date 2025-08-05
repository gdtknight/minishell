/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:06:19 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:03:05 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"
#include "envp.h"
#include "shell_data.h"

t_shell_data	*get_shell_data(void)
{
	static t_shell_data	shell_data;

	return (&shell_data);
}

t_status	init_shell_data(char *envp[])
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
	tcgetattr(STDIN_FILENO, &(get_shell_data()->term));
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 22:12:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 05:34:49 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

#include "def.h"
#include "builtin.h"
#include "execute.h"

/**
 * @brief Executes a builtin command based on the command form.
 *
 * Searches for the command in the builtin table and calls the corresponding
 * function if found.
 *
 * @param cmd_form Pointer to the command form structure.
 */
void	execute_builtin(t_cmd_form *cmd_form)
{
	static t_builtin_entry	builtins[] = {
	{"cd", builtin_cd},
	{"export", builtin_export},
	{"pwd", builtin_pwd},
	{"unset", builtin_unset},
	{"echo", builtin_echo},
	{"env", builtin_env},
	{"exit", builtin_exit},
	};
	size_t					idx;

	if (!cmd_form)
		return ;
	idx = -1;
	while (++idx < sizeof(builtins) / sizeof(t_builtin_entry))
	{
		if (ft_strncmp(builtins[idx].cmd_str, \
				cmd_form->cmd, ft_strlen(builtins[idx].cmd_str) + 1) == 0)
		{
			(builtins[idx].cmd)(cmd_form->args);
			return ;
		}
	}
}

/**
 * @brief Checks if a command string is a builtin command.
 *
 * Compares the command string to the list of supported builtins.
 *
 * @param cmd Command string to check.
 * @return t_boolean TRUE if builtin, FALSE otherwise.
 */
t_boolean	is_builtin(char *cmd)
{
	static char	*builtins[] = {
		"cd",
		"export",
		"pwd",
		"unset",
		"echo",
		"env",
		"exit",
	};
	size_t		idx;

	if (!cmd)
		return (FALSE);
	idx = -1;
	while (++idx < sizeof(builtins) / sizeof(char *))
	{
		if (ft_strncmp(builtins[idx], cmd, ft_strlen(cmd) + 1) == 0)
			return (TRUE);
	}
	return (FALSE);
}

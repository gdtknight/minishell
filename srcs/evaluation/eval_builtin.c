/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 22:12:37 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/09 02:42:36 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "def.h"
#include "eval.h"
#include "builtin.h"

t_status	execute_builtin(t_cmd_form cmd_form)
{
	static t_builtin_entry	builtins[] = {
    	{"cd", builtin_cd_refactor},
    	{"export", builtin_export_refactor},
    	{"pwd", builtin_pwd_refactor},
    	{"unset", builtin_unset_refactor},
    	{"echo", builtin_echo_refactor},
    	{"env", builtin_env_refactor},
    	{"exit", builtin_exit_refactor},
	};
	size_t					idx;

	idx = -1;
	while (++idx < sizeof(builtins) / sizeof(t_builtin_entry))
	{
		if (ft_strncmp(builtins[idx].cmd_str, \
				cmd_form.cmd, ft_strlen(builtins[idx].cmd_str) + 1) == 0)
			return ((builtins[idx].cmd)(cmd_form.args));
	}
	return (FAILURE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:26:48 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 05:21:44 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "def.h"

# include "ast.h"

typedef t_status	(*t_builtin)(char **);

typedef struct s_builtin_entry
{
	char			*cmd_str;
	t_builtin		cmd;
}	t_builtin_entry;

/* --- execute_builtin.c --- */

void		execute_builtin(t_cmd_form *cmd_form);
t_boolean	is_builtin(char *cmd);

/* --- execute_command.c --- */

void		execute_command(t_cmd_form *cmd_form);

/* --- execute_utils.c --- */

char		*find_path(char *cmd, char *envp[]);

#endif

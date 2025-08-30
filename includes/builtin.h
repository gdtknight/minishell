/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:19:29 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/30 09:09:56 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "def.h"

typedef enum e_cd_err
{
	CD_OK = 0,
	CD_TOO_MANY_ARGS,
	CD_HOME_NOT_SET,
	CD_OLDPWD_NOT_SET,
	CD_EACCES,
	CD_ENOENT,
	CD_ENOTDIR,
	CD_UNKNOWN
}	t_cd_err;

typedef enum e_builtin_exit
{
	NO_ARGC = 0,
	WITH_ARGC = 1,
	TOO_MANY_ARGC = 2,
	WRONG_ARGC = 3
}	t_builtin_exit;

typedef enum e_builtin_export
{
	NAME_INVALID = 0,
	NAME_VALID = 1,
	NAME_APPEND = 2,
}	t_builtin_export;

t_status	builtin_cd(char **args);
t_status	builtin_export(char **args);
t_status	builtin_pwd(char **args);
t_status	builtin_unset(char **args);
t_status	builtin_echo(char **args);
t_status	builtin_env(char **args);
t_status	builtin_exit(char **args);

int			count_argc(char **argc);
void		update_pwd(void);

t_boolean	check_arg(char *arg);

#endif

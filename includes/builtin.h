/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:19:29 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/18 03:53:30 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "def.h"

t_status	builtin_cd(char **args);
t_status	builtin_export(char **args);
t_status	builtin_pwd(char **args);
t_status	builtin_unset(char **args);
t_status	builtin_echo(char **args);
t_status	builtin_env(char **args);
t_status	builtin_exit(char **args);

t_status	builtin_cd_refactor(char **args);
t_status	builtin_export_refactor(char **args);
t_status	builtin_pwd_refactor(char **args);
t_status	builtin_unset_refactor(char **args);
t_status	builtin_echo_refactor(char **args);
t_status	builtin_env_refactor(char **args);
t_status	builtin_exit_refactor(char **args);

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

#endif

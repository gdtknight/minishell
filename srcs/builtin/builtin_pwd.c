/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:35:10 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/22 16:48:45 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "builtin.h"
#include "shell.h"

/**
 * @brief Implementation of the builtin pwd command.
 *
 * Prints the current working directory to standard output.
 * @param args The argument array (unused).
 * @return SUCCESS if the directory is printed, ERROR otherwise.
 */
t_status	builtin_pwd(char **args)
{
	char	*cwd;

	(void) args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ERROR);
	printf("%s\n", cwd);
	free(cwd);
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}

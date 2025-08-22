/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:18:39 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/22 16:47:37 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "def.h"

#include "builtin.h"
#include "shell.h"

/**
 * @brief Checks for the -n option(s) in echo arguments.
 *
 * @param args The argument array.
 * @return The index of the first argument that is not an -n option.
 */
static int	check_option(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		i++;
	}
	return (i);
}

/**
 * @brief Implementation of the builtin echo command.
 *
 * @param args The argument array.
 * @return SUCCESS status after printing the arguments.
 */
t_status	builtin_echo(char **args)
{
	int	print_idx;
	int	i;

	if (!args[1])
	{
		get_shell_data()->last_status = 0;
		printf("\n");
		return (SUCCESS);
	}
	print_idx = check_option(args);
	i = print_idx;
	printf("%s", args[i++]);
	while (args[i])
	{
		printf(" %s", args[i]);
		i++;
	}
	if (print_idx == 1)
		printf("\n");
	get_shell_data()->last_status = 0;
	return (SUCCESS);
}

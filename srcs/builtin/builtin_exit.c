/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:44:13 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/22 16:47:20 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

#include "def.h"
#include "builtin.h"
#include "shell.h"

/**
 * @brief Checks if a character is a whitespace character.
 *
 * @param c The character to check.
 * @return 1 if whitespace, 0 otherwise.
 */
static int	ft_isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\v' || c == '\t' || c == '\r')
		return (1);
	return (0);
}

/**
 * @brief Converts a string to a long long integer, sets flag if invalid.
 *
 * @param nptr The string to convert.
 * @param flag_args Pointer to flag for argument validity.
 * @return The converted long long value.
 */
static long long	ft_atoll(const char *nptr, t_builtin_exit *flag_args)
{
	int			flag;
	long long	result;
	long long	temp;

	flag = 1;
	result = 0;
	if (!nptr)
		return (result);
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
		if (*nptr++ == '-')
			flag = -1;
	while (ft_isdigit(*nptr))
	{
		temp = result;
		result *= 10;
		result += (*nptr++) - '0';
		if (temp > result)
			*flag_args = WRONG_ARGC;
	}
	return ((result) * (flag));
}

/**
 * @brief Checks the arguments for the exit command and returns
 * the argument status.
 *
 * @param args The argument array.
 * @return The argument status (t_builtin_exit enum).
 */
static t_builtin_exit	check_args(char **args)
{
	int	i;
	int	j;

	i = 1;
	if (!args[i])
		return (NO_ARGC);
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (!ft_isdigit(args[i][j]))
				return (WRONG_ARGC);
			j++;
		}
		i++;
	}
	if (i > 2)
		return (TOO_MANY_ARGC);
	return (WITH_ARGC);
}

/**
 * @brief Handles the exit status and error messages based on argument flags.
 *
 * @param flag_args The argument status flag.
 * @param args The argument array.
 * @param exit_code The exit code to use if valid.
 */
static void	handle_flag(
				t_builtin_exit flag_args,
				char **args,
				long long exit_code)
{
	get_shell_data()->is_exit = TRUE;
	if (flag_args == WITH_ARGC)
		get_shell_data()->last_status = (int)((exit_code) % 256);
	if (flag_args == WRONG_ARGC)
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		get_shell_data()->last_status = 2;
	}
	if (flag_args == TOO_MANY_ARGC)
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		get_shell_data()->last_status = 1;
		get_shell_data()->is_exit = FALSE;
	}
}

/**
 * @brief Implementation of the builtin exit command.
 *
 * @param args The argument array.
 * @return SUCCESS status after handling exit logic.
 */
t_status	builtin_exit(char **args)
{
	t_builtin_exit	flag_args;
	long long		exit_code;

	ft_putstr_fd ("exit\n", STDOUT_FILENO);
	flag_args = check_args(args);
	exit_code = ft_atoll(args[1], &flag_args);
	handle_flag(flag_args, args, exit_code);
	return (SUCCESS);
}

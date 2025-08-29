/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:30:45 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 05:20:05 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "libft.h"
#include "def.h"
#include "utils.h"

static t_boolean	check_positive(char *arg);
static t_boolean	check_negative(char *arg);

t_boolean	check_arg(char *arg)
{
	char	*s;

	s = arg;
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
		if (!ft_isdigit(*s++))
			return (FALSE);
	if (*arg == '-')
	{
		if (!check_negative(arg))
			return (FALSE);
	}
	else if (!check_positive(arg))
		return (FALSE);
	return (TRUE);
}

static t_boolean	check_positive(char *arg)
{
	t_boolean	result;
	char		*a_llong_max;
	int			idx;

	result = TRUE;
	a_llong_max = ft_lltoa(LLONG_MAX);
	if (ft_strlen(arg) > ft_strlen(a_llong_max))
		result = FALSE;
	else if (ft_strlen(arg) == ft_strlen(a_llong_max))
	{
		idx = -1;
		while (++idx < (int) ft_strlen(a_llong_max))
		{
			if (arg[idx] == a_llong_max[idx])
				continue ;
			if (arg[idx] > a_llong_max[idx])
				result = FALSE;
		}
	}
	free(a_llong_max);
	return (result);
}

static t_boolean	check_negative(char *arg)
{
	t_boolean	status;
	char		*a_llong_min;
	int			idx;

	status = TRUE;
	a_llong_min = ft_lltoa(LLONG_MIN);
	if (ft_strlen(arg) > ft_strlen(a_llong_min))
		status = FALSE;
	else if (ft_strlen(arg) == ft_strlen(a_llong_min))
	{
		idx = -1;
		while (++idx < (int) ft_strlen(a_llong_min))
		{
			if (arg[idx] == a_llong_min[idx])
				continue ;
			if (arg[idx] > a_llong_min[idx])
				status = FALSE;
		}
	}
	free(a_llong_min);
	return (status);
}

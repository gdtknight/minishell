/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_refactor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:49:30 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/11 22:02:20 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "def.h"

#include "builtin.h"

t_status	builtin_exit_refactor(char **args)
{
	(void) args;
	printf("builtin exit executed\n");
	return (SUCCESS);
}

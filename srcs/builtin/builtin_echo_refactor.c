/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_refactor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:47:58 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/11 22:01:40 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "def.h"

#include "builtin.h"

t_status	builtin_echo_refactor(char **args)
{
	(void)args;
	printf("builtin echo executed\n");
	return (SUCCESS);
}

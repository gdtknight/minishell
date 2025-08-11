/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_refactor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:19:53 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/11 22:03:04 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "def.h"

#include "builtin.h"

t_status	builtin_unset_refactor(char **args)
{
	(void)args;
	printf("builtin unset executed\n");
	return (SUCCESS);
}

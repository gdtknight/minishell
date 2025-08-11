/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_refactor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:47:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/11 22:02:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "def.h"

#include "builtin.h"

t_status	builtin_pwd_refactor(char **args)
{
	(void)args;
	printf("builtin pwd executed\n");
	return (SUCCESS);
}

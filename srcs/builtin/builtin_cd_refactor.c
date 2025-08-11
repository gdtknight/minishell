/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_refactor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:47:08 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/11 22:00:41 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "def.h"

#include "builtin.h"

t_status	builtin_cd_refactor(char **args)
{
	(void)args;
	printf("builtin cd executed\n");
	return (SUCCESS);
}

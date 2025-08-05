/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/04 21:52:21 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <stdio.h>

#include "debug.h"
#include "hashmap.h"
#include "def.h"
#include "libft.h"
#include "builtin.h"

/**
 * @brief 해시맵에서 특정 key 값에 해당하는 entry를 삭제한다.
 *
 * 전달받은 문자열 line을 공백 기준으로 분리하여 첫 번째 토큰을 key로 사용하고,
 * 해당 key를 해시맵에서 찾아 삭제한다.
 *
 * @param line  key를 포함한 문자열
 * @param map   entry가 삭제될 대상 해시맵
 * @return SUCCESS(삭제 성공), FAIL(입력값 오류)
 */

void	free_un(char **keys)
{
	int	i;

	i = 0;
	while (keys[i])
	{
		free(keys[i]);
		i++;
	}
	free (keys);
}

int	builtin_unset(char *line, t_hash_map *map)
{
	char			**envp_keys;
	int				i;

	if (!line || line[0] == 0 || !map)
		return (BUILTIN_FAIL);
	envp_keys = ft_split(line, ' ');
	i = 0;
	while (envp_keys[i])
	{
		remove_entry(map, get_entry(map, envp_keys[i]));
		i++;
	}
	free_un(envp_keys);
	return (BUILTIN_SUCCESS);
}



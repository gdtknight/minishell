/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:13:12 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/07 20:11:46 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "hashmap.h"
#include "def.h"
#include "libft.h"
#include "builtin.h"
#include "utils.h"

void	free_ex(char **envps)
{
	int	i;

	i = 0;
	while (envps[i])
	{
		free (envps[i]);
		i++;
	}
	free (envps);
}

t_boolean	name_checker(char *name)
{
	int	i;

	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (FALSE);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_result	check_and_set_enp(char *envp, t_hash_map *map)
{
	char		*key;
	char		*value;
	t_result	result;

	result = COMPLETED;
	key = extract_key(envp);
	value = extract_value(envp);
	if (name_checker(key))
		put_key_value(map, key, value);
	else
	{
		printf("bash: export: `%s': not a valid identifier\n", key);
		result = INCOMPLETED;
	}
	free (key);
	free (value);
	return (result);
}

/**
 * @brief 환경 변수를 해시맵에 추가 또는 갱신한다.
 *
 * 전달받은 문자열 line을 공백 기준으로 분리하여 각 토큰을 key=value 형태로 파싱한다.
 * key가 유효한지 검사 후 해시맵에 삽입하거나 갱신하며, 잘못된 key일 경우 에러 메시지를 출력한다.
 *
 * @param line  추가 또는 갱신할 환경 변수 문자열
 * @param map   환경 변수가 저장될 해시맵
 * @return SUCCESS(추가/갱신 성공), FAIL(잘못된 key 입력 시)
 */
t_status	builtin_export(char *line, t_hash_map *map)
{
	char	**envps;
	int		status;
	int		i;

	envps = ft_split(line, ' ');
	if (errno == ENOMEM)
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	i = 0;
	status = SUCCESS;
	while (envps[i])
	{
		if (check_and_set_enp(envps[i], map) == INCOMPLETED)
			status = FAILURE;
		i++;
	}
	free_ex(envps);
	return (status);
}

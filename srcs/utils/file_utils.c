/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:37:51 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 20:20:40 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "libft.h"

#include "utils.h"

static void	free_string_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	free(arr);
}

char	**match_files_in_dir(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				count;
	int				capacity;
	char			**new_result;
	int				idx;

	count = 0;
	capacity = INIT_CAPACITY;
	result = (char **)ft_calloc(capacity + 1, sizeof(char *));
	if (!result)
		return (NULL);
	dir = opendir(".");
	if (!dir)
	{
		free(result);
		return (NULL);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (wildcard_match(entry->d_name, pattern))
		{
			if (count == capacity)
			{
				capacity *= 2;
				new_result = (char **)ft_calloc(capacity + 1, sizeof(char *));
				if (!new_result)
				{
					free_string_array(result);
					closedir(dir);
					return (NULL);
				}
				idx = -1;
				while (++idx < count)
				{
					new_result[idx] = result[idx];
				}
				free(result);
				result = new_result;
			}
			result[count++] = ft_strdup(entry->d_name);
		}
		entry = readdir(dir);
	}
	result[count] = NULL;
	closedir(dir);
	return (result);
}

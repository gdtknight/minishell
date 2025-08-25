/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:33:51 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 17:03:28 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "expand.h"

char	*get_homedir(char *username)
{
	char	*home_dir;
	char	*home_prefix;

	home_prefix = get_home_prefix();
	home_dir = ft_strjoin(home_prefix, username);
	free(username);
	free(home_prefix);
	return (home_dir);
}

t_boolean	check_homedir(char *username)
{
	t_boolean	result;
	char		*home_prefix;
	char		*home_dir;

	result = FALSE;
	home_prefix = get_home_prefix();
	home_dir = ft_strjoin(home_prefix, username);
	if (access(home_dir, R_OK) == 0)
		result = TRUE;
	free(home_dir);
	free(home_prefix);
	return (result);
}

char	*get_home_prefix(void)
{
	char	*home_path;
	char	*last_slash;

	home_path = getenv("HOME");
	last_slash = ft_strrchr(home_path, '/');
	return (ft_substr(
			home_path,
			0,
			ft_strlen(home_path) - ft_strlen(last_slash) + 1));
}

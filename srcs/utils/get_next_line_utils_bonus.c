/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:01:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 06:03:10 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*read_line(t_buffer *bcur, char **line, t_flag *empty)
{
	ssize_t	read_len;

	read_len = BUFFER_SIZE;
	while (read_len > 0)
	{
		*line = gnl_strjoin(*line, read_buffer(bcur));
		if (bcur->eidx == bcur->sidx)
		{
			bcur->sidx = 0;
			bcur->eidx = 0;
		}
		if (line && gnl_strnchr(*line, '\n', gnl_strlen(*line)) != (-1))
			break ;
		read_len = read(bcur->fd, bcur->buffer, BUFFER_SIZE);
		bcur->eidx += read_len;
	}
	if (bcur->eidx == bcur->sidx)
		*empty = 1;
	if (read_len == -1 && *line)
		release_str(line);
	return (*line);
}

size_t	gnl_strlen(const char *src)
{
	const char	*s;

	if (!src)
		return (0);
	s = src;
	while (*s)
		s++;
	return (s - src);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	char	*str;
	char	*ss1;
	char	*ss2;
	size_t	idx;

	if (!s1 && !s2)
		return (NULL);
	str = (char *) malloc(sizeof(char) * (gnl_strlen(s1) + gnl_strlen(s2) + 1));
	if (!str)
	{
		release_str(&s1);
		release_str(&s2);
		return (NULL);
	}
	idx = 0;
	ss1 = s1;
	ss2 = s2;
	while (s1 && *s1)
		str[idx++] = *s1++;
	while (s2 && *s2)
		str[idx++] = *s2++;
	str[idx] = '\0';
	release_str(&ss1);
	release_str(&ss2);
	return (str);
}

int	gnl_strnchr(char *str, char c, int len)
{
	int	idx;

	if (!str)
		return (-1);
	idx = -1;
	while (++idx < len)
		if (str[idx] == c)
			return (idx);
	return (-1);
}

void	release_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:05:47 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/28 17:00:38 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_buffer	*blst;
	t_buffer		*bcur;
	char			*line;
	t_flag			empty;

	bcur = NULL;
	line = NULL;
	empty = 0;
	if (!init(&blst, &bcur, fd))
		return (NULL);
	line = read_line(bcur, &line, &empty);
	clear_buffer(&blst, fd);
	return (line);
}

int	init(t_buffer **blst, t_buffer **bcur, int fd)
{
	if (fd < 0 || fd == 1 || fd == 2 || BUFFER_SIZE <= 0)
		return (0);
	*bcur = *blst;
	while (*bcur)
	{
		if ((*bcur)->fd == fd)
			return (1);
		(*bcur) = (*bcur)->next;
	}
	*bcur = create_new_buf(fd);
	if (!*bcur)
		return (0);
	if (!(*blst))
		*blst = *bcur;
	else
	{
		(*bcur)->next = *blst;
		*blst = *bcur;
	}
	return (1);
}

t_buffer	*create_new_buf(int fd)
{
	t_buffer	*new_buf;

	new_buf = (t_buffer *) malloc(sizeof(t_buffer));
	if (!new_buf)
		return (NULL);
	new_buf->buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE);
	if (!(new_buf->buffer))
	{
		free (new_buf);
		return (NULL);
	}
	new_buf->sidx = 0;
	new_buf->eidx = 0;
	new_buf->fd = fd;
	new_buf->next = NULL;
	return (new_buf);
}

void	clear_buffer(t_buffer **blst, int fd)
{
	t_buffer	*bprev;
	t_buffer	*bcur;
	t_buffer	*bnext;

	if (!blst)
		return ;
	bprev = NULL;
	bcur = (*blst);
	while (bcur)
	{
		bnext = bcur->next;
		if (bcur->fd == fd)
		{
			if (bprev)
				bprev->next = bnext;
			else
				*blst = bnext;
			free(bcur->buffer);
			bcur->buffer = NULL;
			free(bcur);
			bcur = NULL;
		}
		bprev = bcur;
		bcur = bnext;
	}
}

char	*read_buffer(t_buffer *buffer)
{
	char	*content;
	size_t	len;
	size_t	idx;

	len = buffer->eidx - buffer->sidx;
	if (len == 0)
		return (NULL);
	if (gnl_strnchr(&((buffer->buffer)[buffer->sidx]), '\n', len) != (-1))
		len = gnl_strnchr(&((buffer->buffer)[buffer->sidx]), '\n', len) + 1;
	content = (char *) ft_calloc(sizeof(char), (len + 1));
	if (!content)
	{
		buffer->eidx = buffer->sidx;
		return (NULL);
	}
	idx = -1;
	while (++idx < len)
	{
		content[idx] = buffer->buffer[buffer->sidx];
		buffer->buffer[buffer->sidx++] = 0;
	}
	content[idx] = '\0';
	return (content);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:04:48 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 06:03:28 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**************
 * Bonus Part *
 **************/

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

/* ******************* *
 *  Default Libraries  *
 * ******************* */

# include <unistd.h>
# include <string.h>
# include <stdlib.h>

/* ************************** *
 *  Default Read Buffer Size  *
 * ************************** */

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

/* **************************** *
 *  Check flag - empty buffer?  *
 * **************************** */

# ifndef EMPTY_FLAG
#  define EMPTY_FLAG

typedef int	t_flag;

# endif

/* *************** *
 *  Buffer Struct  *
 * *************** */

typedef struct s_buffer
{
	int				fd;
	char			*buffer;
	size_t			sidx;
	size_t			eidx;
	struct s_buffer	*next;
}	t_buffer;

/* ***************** *
 *  get_next_line.c  *
 * ***************** */

char		*get_next_line(int fd);
int			init(t_buffer **blst, t_buffer **bcur, int fd);
char		*read_buffer(t_buffer *buffer);
t_buffer	*create_new_buf(int fd);
void		clear_buffer(t_buffer **blst, int fd);

/* *********************** *
 *  get_next_line_utils.c  *
 * *********************** */

char		*read_line(t_buffer *bcur, char **line, t_flag *empty);
size_t		gnl_strlen(const char *src);
char		*gnl_strjoin(char *s1, char *s2);
int			gnl_strnchr(char *str, char c, int len);
void		release_str(char **str);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:13:41 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 19:43:03 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "def.h"

#include "libft.h"

char	*heredoc(const char *limiter)
{
	int		tty_fd;
	char	*input;
	char	*line;
	char	*temp;

	tty_fd = open("/dev/tty", O_WRONLY);
	if (tty_fd == -1)
	{
	    perror("open /dev/tty");
		return (NULL);
	}
	input = NULL;
	while (TRUE)
	{
		write(tty_fd, "heredoc> ", 9);
		line = readline("");
		if (ft_strncmp(line, limiter, ft_strlen(line) + 1) == 0)
			break ;
		temp = input;
		if (temp)
		{
			input = ft_strjoin(temp, line);
			free(temp);
		}
		else
			input = ft_strdup(line);
		free(line);
		temp = input;
		input = ft_strjoin(temp, "\n");
		free(temp);
	}
	close(tty_fd);
	return (input);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:13:41 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/13 11:41:58 by jyoo             ###   ########.fr       */
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

static void	read_input(char **input, const char *limiter, int tty_fd);
static int	set_heredoc_fd(void);

char	*heredoc(const char *limiter)
{
	int		tty_fd;
	char	*input;

	input = NULL;
	tty_fd = set_heredoc_fd();
	read_input(&input, limiter, tty_fd);
	close(tty_fd);
	return (input);
}

static void	read_input(char **input, const char *limiter, int tty_fd)
{
	char	*temp;
	char	*line;

	while (TRUE)
	{
		write(tty_fd, "heredoc> ", 9);
		line = readline("");
		if (ft_strncmp(line, limiter, ft_strlen(line) + 1) == 0)
			break ;
		temp = *input;
		if (temp)
		{
			*input = ft_strjoin(temp, line);
			free(temp);
		}
		else
			*input = ft_strdup(line);
		free(line);
		temp = *input;
		*input = ft_strjoin(temp, "\n");
		free(temp);
	}
}

static int	set_heredoc_fd(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_WRONLY);
	if (tty_fd == -1)
	{
		perror("open /dev/tty");
		exit(EXIT_FAILURE);
	}
	return (tty_fd);
}

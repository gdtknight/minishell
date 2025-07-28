/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:13:41 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/26 21:26:19 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <stdlib.h>

#include "libft.h"

char	*heredoc(const char *limiter)
{
	char	*input;
	char	*line;
	char	*temp;

	input = NULL;
	line = readline("> ");
	while (ft_strncmp(line, limiter, ft_strlen(line)) != 0)
	{
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
		line = readline("> ");
	}
	return (input);
}

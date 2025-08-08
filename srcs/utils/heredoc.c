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

#include <readline/readline.h>
#include <stdlib.h>

#include "libft.h"

char	*heredoc(const char *limiter)
{
	char	*input;
	char	*line;
	char	*temp;

	input = NULL;
	line = readline("heredoc> ");
	while (ft_strncmp(line, limiter, ft_strlen(line) + 1) != 0)
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
		line = readline("heredoc> ");
	}
	return (input);
}

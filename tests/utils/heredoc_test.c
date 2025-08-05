/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:59:13 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:33:10 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int	main(void)
{
	static char	*limiter = "here";
	char		*input;

	input = heredoc(limiter);
	printf("---- result ----\n");
	printf("%s", input);
	printf("----------------\n");
	free(input);
	exit(EXIT_SUCCESS);
}

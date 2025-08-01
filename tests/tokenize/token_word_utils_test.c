/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_utils_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:26:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/01 17:39:03 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "libft.h"

#include "tokenizer.h"

void	test_extract_normal_word(void)
{
	static char	*test = "cmd1;-option1";
	static char	*result = "cmd1";

	if (ft_strncmp(extract_normal_word(test), result, ft_strlen(result)) == 0)
		printf("extract_normal_word() - SUCCESS\n");
	else
		printf("extract_normal_word() - FAIL\n");
	printf("extract_normal_word() result : %s\n", result);
}

int	main(void)
{
	test_extract_normal_word();
	exit(EXIT_SUCCESS);
}

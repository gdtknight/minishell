/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_utils_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:26:35 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/16 21:46:09 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "libft.h"

#include "tokenizer.h"

void	test_extract_squote_word(void)
{
	char	*test = "'cmd1 -option1'";
	char	*result = "cmd1 -option1";

	if (ft_strncmp(extract_squote_word(test), result, ft_strlen(result)) == 0)
		printf("extract_squote_word() - SUCCESS\n");
	else
		printf("extract_squote_word() - FAIL\n");
	printf("extract_squote_word() result : %s\n", result);
}

void	test_extract_dquote_word(void)
{
	char	*test = "\"cmd1 -option1\"";
	char	*result = "cmd1 -option1";

	if (ft_strncmp(extract_dquote_word(test), result, ft_strlen(result)) == 0)
		printf("extract_dquote_word() - SUCCESS\n");
	else
		printf("extract_dquote_word() - FAIL\n");
	printf("extract_dquote_word() result : %s\n", result);
}

void	test_extract_normal_word(void)
{
	char	*test = "cmd1;-option1";
	char	*result = "cmd1";

	if (ft_strncmp(extract_normal_word(test), result, ft_strlen(result)) == 0)
		printf("extract_normal_word() - SUCCESS\n");
	else
		printf("extract_normal_word() - FAIL\n");
	printf("extract_normal_word() result : %s\n", result);
}


int	main(void)
{
	test_extract_squote_word();
	test_extract_dquote_word();
	test_extract_normal_word();

	exit(EXIT_SUCCESS);
}

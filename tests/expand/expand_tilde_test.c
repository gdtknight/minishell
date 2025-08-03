/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:23:20 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/02 21:34:06 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "expand.h"
#include "libft.h"
#include "tokenizer.h"

void	expand_tilde_success_test(void)
{
	const char	*home_dir = getenv("HOME");
	char	*before_value = "test root : ~";
	char	*after_value = ft_strjoin("test root : ", home_dir);
	t_token	*token;

	token = create_empty_token();
	token->type = TK_WORD;
	token->value = ft_strdup(before_value);
	token = expand_tilde(token);
	if (ft_strncmp(token->value, after_value, ft_strlen(token->value)) == 0)
	{
		printf("[SUCCESS] before_value : \"%s\", after_value : \"%s\"\n",
			before_value, (char *)(token->value));
	}
	free(after_value);
	free(token->value);
	free(token);
}

int	main(void)
{
	expand_tilde_success_test();
	exit(EXIT_SUCCESS);
}

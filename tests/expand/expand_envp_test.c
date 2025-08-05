/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envp_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 21:24:49 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 16:31:17 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"
#include "libft.h"

#include "expand.h"
#include "shell_data.h"
#include "tokenizer.h"

void	expand_envp_dot_test(void)
{
	static const char	*dot_str = "$a2.txt";
	t_token				*test_token;

	test_token = create_empty_token();
	test_token->type = TK_WORD;
	test_token->value = ft_strdup(dot_str);
	test_token = expand_envp(test_token);
	if (ft_strlen(test_token->value) == ft_strlen(".txt")
		&& (ft_strncmp(test_token->value, \
				".txt", ft_strlen(test_token->value)) == 0))
		printf("[SUCCESS] before value : \"%s\", after value : \"%s\"\n",
			dot_str, (char *)(test_token->value));
	else
		printf("[FAIL] before value : \"%s\", after value : \"%s\"\n",
			dot_str, (char *)(test_token->value));
	free(test_token->value);
	free(test_token);
}

void	expand_envp_fail1(void)
{
	const char	*path_var = ft_strjoin("SHLVL:", getenv("SHLVL"));
	t_token		*token;

	token = create_empty_token();
	token->type = TK_WORD;
	token->value = ft_strdup("SHLVL:\\$SHLVL");
	token = expand_envp(token);
	if (ft_strncmp(token->value, path_var, ft_strlen(token->value)) != 0)
		printf("[SUCCESS] before value : \"%s\", after value : \"%s\"\n",
			"SHLVL:\\$SHLVL", (char *)(token->value));
	free(token->value);
	free(token);
	free((void *)path_var);
}

void	expand_envp_success1(void)
{
	const char	*path_var = ft_strjoin("SHLVL:", getenv("SHLVL"));
	t_token		*token;

	token = create_empty_token();
	token->type = TK_WORD;
	token->value = ft_strdup("SHLVL:$SHLVL");
	token = expand_envp(token);
	if (ft_strncmp(token->value, path_var, ft_strlen(token->value)) == 0)
		printf("[SUCCESS] before value : \"%s\", after value : \"%s\"\n",
			"SHLVL:$SHLVL", (char *)(token->value));
	else
		printf("[FAIL] before value : \"%s\", after value : \"%s\"\n",
			"SHLVL:$SHLVL", (char *)(token->value));
	free(token->value);
	free(token);
	free((void *)path_var);
}

void	expand_envp_success2(void)
{
	const char	*path_var = getenv("PATH");
	t_token		*token;

	token = create_empty_token();
	token->type = TK_WORD;
	token->value = ft_strdup("$PATH");
	token = expand_envp(token);
	if (ft_strncmp(token->value, path_var, ft_strlen(token->value)) == 0)
		printf("[SUCCESS] before value : \"%s\", after value : \"%s\"\n",
			"$PATH", (char *)(token->value));
	else
		printf("[FAIL] before value : \"%s\", after value : \"%s\"\n",
			"$PATH", (char *)(token->value));
	free(token->value);
	free(token);
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	init_shell_data(envp);
	expand_envp_success1();
	printf("\n\n");
	expand_envp_success2();
	printf("\n\n");
	expand_envp_fail1();
	printf("\n\n");
	expand_envp_dot_test();
	clear_hashmap(&(get_shell_data()->envp_map));
	exit(EXIT_SUCCESS);
}

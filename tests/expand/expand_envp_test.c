/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envp_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 02:10:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 12:54:51 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "expand.h"
#include "shell.h"

#include "debug.h"

void	expand_envp_success_test1(void)
{
	char	*test_str1 = "$0";
	char	*test_str2 = "$?";
	char	*test_str3 = "abc$0def";
	char	*test_str4 = "abc$?def";

	char	*expanded_value1 = expand_envp(test_str1);
	char	*expanded_value2 = expand_envp(test_str2);
	char	*expanded_value3 = expand_envp(test_str3);
	char	*expanded_value4 = expand_envp(test_str4);

	debug("test_str1 : %s, value1 : %s", test_str1, expanded_value1);
	debug("test_str2 : %s, value2 : %s", test_str2, expanded_value2);
	debug("test_str3 : %s, value3 : %s", test_str3, expanded_value3);
	debug("test_str4 : %s, value4 : %s", test_str4, expanded_value4);

	free(expanded_value1);
	free(expanded_value2);
	free(expanded_value3);
	free(expanded_value4);
}

void	expand_envp_success_test2(void)
{
	char	*test_str1 = "abc$USERdef";
	char	*test_str2 = "abc\'$USER\'def";
	char	*test_str3 = "abc\"$USER\"def";

	char	*expanded_value1 = expand_envp(test_str1);
	char	*expanded_value2 = expand_envp(test_str2);
	char	*expanded_value3 = expand_envp(test_str3);

	debug("test_str1 : %s, value1 : %s", test_str1, expanded_value1);
	debug("test_str2 : %s, value2 : %s", test_str2, expanded_value2);
	debug("test_str3 : %s, value3 : %s", test_str3, expanded_value3);

	free(expanded_value1);
	free(expanded_value2);
	free(expanded_value3);
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	init_shell_data(envp);
	expand_envp_success_test1();
	printf("\n");
	expand_envp_success_test2();
	clear_shell_data();
	exit(EXIT_SUCCESS);
}

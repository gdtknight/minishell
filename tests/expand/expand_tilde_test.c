/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:23:20 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 05:07:16 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "expand.h"
#include "libft.h"

#include "debug.h"

void	expand_tilde_success_test1(void)
{
	char	*tilde = "~";
	char	*tildeplus = "~+";
	char	*tildeminus = "~-";
	char	*new_value;

	new_value = expand_tilde(tilde);
	if (ft_strncmp(getenv("HOME"), new_value, ft_strlen(new_value) + 1) == 0)
		debug("\"~\" is expanded - before : %s, after - : %s", tilde, new_value);
	free(new_value);
	new_value = expand_tilde(tildeplus);
	if (ft_strncmp(getenv("PWD"), new_value, ft_strlen(new_value) + 1) == 0)
		debug("\"~+\" is expanded - before : %s, after - : %s", tildeplus, new_value);
	free(new_value);
	new_value = expand_tilde(tildeminus);
	if (ft_strncmp(getenv("OLDPWD"), new_value, ft_strlen(new_value) + 1) == 0)
		debug("\"~-\" is expanded - before : %s, after - : %s", tildeminus, new_value);
	free(new_value);
}

void	expand_tilde_success_test2(void)
{
	char	*username;
	char	*tilde_with_username;
	char	*new_value;

	username = ft_strdup(ft_strrchr(getenv("HOME"), '/') + 1);
	tilde_with_username = ft_strjoin("~", username);
	new_value = expand_tilde(tilde_with_username);
	if (ft_strncmp(getenv("HOME"), new_value, ft_strlen(new_value) + 1) == 0)
		debug("\"%s\" is expanded - before : %s, after - : %s",
			tilde_with_username,
			tilde_with_username,
			new_value);
	free(username);
	free(tilde_with_username);
	free(new_value);
}

void	expand_tilde_success_test3(void)
{
	char	*unknown_user;
	char	*tilde_with_username;
	char	*new_value;

	unknown_user = ft_strdup("unknown_user");
	tilde_with_username = ft_strjoin("~", unknown_user);
	new_value = expand_tilde(tilde_with_username);
	if (ft_strncmp(tilde_with_username, new_value, ft_strlen(new_value) + 1) == 0)
		debug("\"%s\" is expanded - before : %s, after - : %s",
			tilde_with_username,
			tilde_with_username,
			new_value);
	free(unknown_user);
	free(tilde_with_username);
	free(new_value);
}

int	main(void)
{
	expand_tilde_success_test1();
	debug("\n");
	expand_tilde_success_test2();
	debug("\n");
	expand_tilde_success_test3();
	exit(EXIT_SUCCESS);
}

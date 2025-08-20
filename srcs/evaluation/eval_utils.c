/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 03:30:48 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 03:31:29 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>

#include "shell_data.h"
#include "eval.h"

#include "debug.h"

void	wait_child(pid_t child_pid, int *status, int options)
{
	int		sig;

	(void)sig;
	waitpid(child_pid, status, options);
	if (WIFEXITED(*status))
	{
		debug("[wait_child (%d)] child (%d) normal exit", getpid(), child_pid);
		(get_shell_data())->last_status = WEXITSTATUS(*status);
	}
	if (WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		debug("[wait_child (%d)] child (%d) killed by signal %d (%s)\n", getpid(), child_pid, sig, strsignal(sig));
		(get_shell_data())->last_status = 128 + WTERMSIG(*status);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 03:30:48 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/21 08:44:35 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>

#include "shell.h"
#include "eval.h"

void	wait_child(pid_t child_pid, int *status, int options)
{
	waitpid(child_pid, status, options);
	if (WIFEXITED(*status))
		(get_shell_data())->last_status = WEXITSTATUS(*status);
	if (WIFSIGNALED(*status))
		(get_shell_data())->last_status = 128 + WTERMSIG(*status);
}

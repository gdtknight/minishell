/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty_restore.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:36:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:32:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

/**
 * @file tty_restore.c
 * @brief Restore standard input/output to the controlling terminal.
 *
 * This function ensures that STDIN and STDOUT are connected to a terminal
 * (tty). If they are not (e.g., redirected from a file or pipe), it opens
 * /dev/tty and duplicates its file descriptor to STDIN_FILENO or STDOUT_FILENO.
 *
 * @note
 * - Uses isatty() to check whether the file descriptors are terminals.
 * - Uses dup2() to redirect the file descriptors to the terminal.
 * - Closes the temporary file descriptor after duplication.
 */
void	restore_tty(void)
{
	int	fd;

	if (!isatty(STDIN_FILENO))
	{
		fd = open("/dev/tty", O_RDONLY);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (!isatty(STDOUT_FILENO))
	{
		fd = open("/dev/tty", O_WRONLY);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

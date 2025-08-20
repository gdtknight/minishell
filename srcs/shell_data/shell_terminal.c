/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_terminal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:36:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 02:36:40 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

void	restore_tty(void)
{
    /* STDIN, STDOUT이 터미널이 아니면 복구 시도 */
    if (!isatty(STDIN_FILENO)) {
        int fd = open("/dev/tty", O_RDONLY);
        if (fd != -1) {
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
    }
    if (!isatty(STDOUT_FILENO)) {
        int fd = open("/dev/tty", O_WRONLY);
        if (fd != -1) {
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
    }
}

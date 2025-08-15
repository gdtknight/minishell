/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:03:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:58:30 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_DATA_H
# define SHELL_DATA_H

# include <signal.h>
# include <termios.h>

# include "def.h"
# include "hashmap.h"
# include "color.h"

# define PROMPT		(AQUA"minishell> "RESET)

typedef struct s_shell_data
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct termios		termios_backup;
	t_hash_map			envp_map;
	int					last_status;
	int					stdin_fd;
	int					stdout_fd;
	t_boolean			in_pipe;
}	t_shell_data;

t_shell_data	*get_shell_data(void);
t_result		init_shell_data(char *envp[]);
void			clear_shell_data(void);

void			save_terminal_settings(void);
void			restore_terminal_settings(void);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:03:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 20:19:19 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <signal.h>
# include <termios.h>

# include "ast.h"
# include "def.h"
# include "hashmap.h"

# define PROMPT		("\033[38;5;14mminishell> \033[0m")

typedef struct s_shell_data
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct termios		termios_backup;
	t_hash_map			envp_map;
	int					last_status;
	pid_t				last_bg_pid;
	char				*last_arg;
	t_boolean			has_child;
	t_boolean			is_child;
	t_boolean			in_heredoc;
	t_boolean			is_exit;
}	t_shell_data;

typedef struct s_shell_input
{
	char			*input_line;
	t_token			*input_token;
	t_syntax_node	*input_node;
}	t_shell_input;

/* --- shell_data.c --- */

t_shell_data	*get_shell_data(void);
t_result		init_shell_data(char *envp[]);
void			clear_shell_data(void);

/* --- shell_input.c --- */

t_shell_input	*get_shell_input(void);
void			clear_shell_input(void);

void			turnoff_input_node_eval(void);

char			**get_heredoc_input(void);
void			clear_heredoc_input(void);

/* --- shell_signal.c --- */

void			init_minishell_signal(void);
void			init_heredoc_signal(void);
void			restore_signal(void);

/* --- shell_signal_handler.c --- */

void			minishell_sigint_handler(int signo);
void			heredoc_sigint_handler(int signo);

/* --- shell_terminal.c --- */

void			restore_tty(void);

#endif

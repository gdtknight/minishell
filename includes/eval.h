/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:14:25 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 02:23:27 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include <unistd.h>

# include "ast.h"

typedef enum e_child_fd
{
	CHILD_LEFT = 0,
	CHILD_RIGHT = 1,
}	t_child_fd;

typedef enum e_pipe_fd
{
	PIPE_READ = 0,
	PIPE_WRITE = 1,
}	t_pipe_fd;

typedef enum e_exit_code
{
	PERMISSION_DENIED_CODE = 126,
	COMMAND_NOT_FOUND_CODE = 127,
}	t_exit_code;

/* --- eval.c --- */

void			eval(t_syntax_node *node);

/* --- eval_list.c --- */

void			eval_list(t_syntax_node *list_node);

/* --- eval_and_or.c --- */

void			eval_and_or(t_syntax_node *and_or_node);

/* --- eval_command.c --- */

void			eval_command(t_syntax_node *cmd_node);

/* --- eval_command_utils.c --- */

char			**get_args_from_suffix(t_syntax_node *cmd_suffix);
void			clear_cmd_form(t_cmd_form *cmd_form);
t_status		set_io_from_prefix(t_syntax_node *prefix);
t_status		set_io_from_suffix(t_syntax_node *suffix);

/* --- eval_heredoc.c --- */

void			eval_heredoc(t_syntax_node *node);

/* --- eval_heredoc_utils.c --- */

void			read_heredoc(t_syntax_node *node);

/* --- eval_io_redir.c --- */

t_status		eval_io_redir(t_syntax_node *io_redir_node);

/* --- eval_io_redir_utils.c --- */

t_status		set_stdin(t_syntax_node *io_redir_node);
t_status		set_stdout(t_syntax_node *io_redir_node);
t_status		set_heredoc(t_syntax_node *io_redir_node);

/* --- eval_pipeline.c --- */

void			eval_pipeline(t_syntax_node	*pipeline_node);

/* --- eval_utils.c --- */

void			wait_child(pid_t child_pid, int *status, int options);
t_syntax_node	*find_cmd_node(t_syntax_node *io_redir_node);

#endif

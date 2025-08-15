/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:14:25 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 22:35:24 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include <unistd.h>

# include "parser.h"

# define PIPE_READ		(0)
# define PIPE_WRITE		(1)
# define CHILD_LEFT		(0)
# define CHILD_RIGHT	(1)

# define PERMISSION_DENIED_CODE	(126)
# define COMMAND_NOT_FOUND_CODE	(127)

typedef struct s_cmd_form
{
	char	*cmd;
	char	**args;
	char	**envp;
}	t_cmd_form;

typedef t_status	(*t_builtin)(char **);

typedef struct s_builtin_entry
{
	char			*cmd_str;
	t_builtin		cmd;
}	t_builtin_entry;

/* --- eval.c --- */

t_status	eval(t_syntax_node *node);

/* --- eval_list.c --- */

t_status	eval_list(t_syntax_node *list_node);

/* --- eval_and_or.c --- */

t_status	eval_and_or(t_syntax_node *and_or_node);

/* --- eval_command.c --- */

t_status	eval_command(t_syntax_node *cmd_node);

/* --- eval_command_utils.c --- */

char		**get_args_from_suffix(t_syntax_node *cmd_suffix);
char		*find_path(char *cmd, char *envp[]);

/* --- eval_io_redir.c --- */

t_status	eval_io_redir(t_syntax_node *io_redir_node);

/* --- eval_pipeline.c --- */

t_status	eval_pipeline(t_syntax_node	*pipeline_node);

/* --- eval_utils.c --- */

int			get_last_exit_code(int status);

/* --- eval_builtin.c --- */
t_status	execute_builtin(t_cmd_form cmd_form);
t_boolean	is_builtin(char *cmd);

/* --- eval_utils --- */
t_status	wait_child(pid_t child_pid, int *status, int options);

#endif

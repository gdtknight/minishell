/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:14:25 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 15:47:32 by yoshin           ###   ########.fr       */
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

typedef struct s_cmd_form
{
	char	*cmd;
	char	**args;
	char	**envp;
}	t_cmd_form;

/* --- eval.c --- */

int			eval(t_syntax_node *node);

/* --- eval_list.c --- */

int			eval_list(t_syntax_node *node);

/* --- eval_and_or.c --- */

int			eval_and_or(t_syntax_node * node);

/* --- eval_command.c --- */

int			eval_command(t_syntax_node *cmd_node);

/* --- eval_command_utils.c --- */

char		**get_args_from_suffix(t_syntax_node *cmd_suffix);
int			count_args(t_syntax_node *cmd_suffix);
char		*find_path(char *cmd, char *envp[]);

/* --- eval_io_redir.c --- */

int			eval_io_redir(t_syntax_node *io_redir_node);

/* --- eval_pipeline.c --- */

int			eval_pipeline(t_syntax_node	*pipeline_node);

/* --- eval_utils.c --- */

#endif

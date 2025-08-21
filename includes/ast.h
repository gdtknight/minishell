/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:19:50 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/20 17:49:22 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "tokenizer.h"

typedef enum e_node_type
{
	NODE_LIST,
	NODE_SEMICOLON,
	NODE_AMPERSAND,
	NODE_AND_IF,
	NODE_OR_IF,
	NODE_PIPELINE,
	NODE_PIPELINE_ERR,
	NODE_SIMPLE_COMMAND,
	NODE_COMPOUND_COMMAND,
	NODE_CMD_PREFIX,
	NODE_CMD_SUFFIX,
	NODE_IO_REDIR_OUT,
	NODE_IO_REDIR_APPEND,
	NODE_IO_REDIR_IN,
	NODE_IO_REDIR_HEREDOC,
	NODE_ASSIGN_WORD,
	NODE_WORD
}	t_node_type;

typedef struct s_cmd_form
{
	char	*cmd;
	char	**args;
	char	**envp;
}	t_cmd_form;

typedef struct s_command
{
	int						heredoc_fds[2];
	struct s_syntax_node	*prefix;
	char					*word;
	struct s_syntax_node	*suffix;
	t_cmd_form				form;
}	t_command;

typedef struct s_binary_node
{
	struct s_syntax_node	*left;
	struct s_syntax_node	*right;
}	t_binary_node;

typedef union u_node_value
{
	char					*assign_word;
	char					*word;
	char					*io_target;
	t_command				command;
	struct s_syntax_node	*child;
	t_binary_node			b_node;
}	t_node_value;

typedef struct s_syntax_node
{
	struct s_syntax_node	*parent;
	t_node_type				type;
	t_node_value			value;
	t_toggle				eval;
}	t_syntax_node;

/* --- parse_input.c --- */

t_syntax_node		*parse_input(t_token **tk_lst);

/* --- construct_node.c --- */

t_syntax_node		*list(t_token **tk_lst);
t_syntax_node		*and_or(t_token **tk_lst);
t_syntax_node		*pipeline(t_token **tk_lst);

/* --- cmd_node.c --- */

t_syntax_node		*command(t_token **tk_lst);
t_syntax_node		*simple_command(t_token **tk_lst);
t_syntax_node		*cmd_prefix(t_token **tk_lst);
t_syntax_node		*cmd_suffix(t_token **tk_lst);

/* --- io_redir_node --- */

t_syntax_node		*io_redir(t_token **tk_lst);

/* --- word_node.c --- */

t_syntax_node		*word(t_token **tk_lst);
t_syntax_node		*assignment_word(t_token **tk_lst);

/* --- node_utils.c --- */

t_syntax_node		*create_empty_node(void);
void				remove_syntax_node(t_syntax_node *target);
void				print_node(t_syntax_node *node, int depth);
void				turnoff_node_eval(t_syntax_node *node);

#endif

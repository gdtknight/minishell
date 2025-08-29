/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:25:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 06:39:02 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stdlib.h>

# include "def.h"

# define STR_EOF			"EOF"
# define STR_BLANK			"BLANK"
# define STR_NEWLINE		"\n"
# define STR_SEMICOLON		";"
# define STR_AMPERSAND		"&"
# define STR_PIPE			"|"
# define STR_PIPE_ERR		"|&"
# define STR_REDIR_IN		"<"
# define STR_REDIR_OUT		">"
# define STR_REDIR_HEREDOC	"<<"
# define STR_REDIR_APPEND	">>"
# define STR_NEWLINE		"\n"

/* ----- Bonus Part ----- */

# define STR_LPAREN			"("
# define STR_RPAREN			")"
# define STR_AND_IF			"&&"
# define STR_OR_IF			"||"

typedef enum e_token_type
{
	TK_EOF,
	TK_NEWLINE,
	TK_BLANK,
	TK_WORD,
	TK_ASSIGN_WORD,
	TK_SEMICOLON,
	TK_AMPERSAND,
	TK_PIPE,
	TK_PIPE_ERR,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_REDIR_HEREDOC,
	TK_REDIR_APPEND,
	TK_LPAREN,
	TK_RPAREN,
	TK_AND_IF,
	TK_OR_IF,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	void			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

/* --- token_factory.c --- */

t_token			*create_empty_token(void);
t_token			*create_token(char *str);

/* --- token_list.c --- */

t_result		clear_token_lst(t_token **lst);
t_result		append_token_to_lst(t_token **lst, t_token *token);
t_result		remove_token_from_lst(t_token *lst, t_token *target);
t_token			*get_last_token(t_token	**lst);

/* --- token_printer.c --- */

t_boolean		match(t_token	*token, t_token_type tk_type);
void			print_token(t_token *token);

/* --- token_validatation.c --- */

t_boolean		is_valid_sequence(t_token *token_lst);
t_boolean		is_io_token(t_token *token);
t_boolean		is_op_token(t_token *token);
t_boolean		is_word_token(t_token *token);

/* --- tokenizer.c --- */

t_token			*tokenize_input(char *line);

/* --- tokenizer_utils.c --- */

char		*find_next_delim(
				char *str,
				t_boolean (*predicate)(char),
				char mask);
size_t		find_next_delim_pos(
				char *str,
				t_boolean (*predicate)(char),
				char mask);

/* --- tokenizer_value.c --- */

t_token_type	get_token_type(char *str);
void			set_token_value_from_str(t_token *target, char *str);
void			set_token_value_from_type(t_token *target);

#endif

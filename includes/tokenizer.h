/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:25:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/24 21:36:06 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "def.h"

# define C_SQUOTE			(1 << 0)
# define C_DQUOTE			(1 << 1)
# define C_BACKSLASH		(1 << 2)

# define STR_EOF			("EOF")
# define STR_BLANK			("BLANK")
# define STR_NEWLINE		("\n")
# define STR_SEMICOLON		(";")
# define STR_AMPERSAND		("&")
# define STR_PIPE			("|")
# define STR_PIPE_ERR		("|&")
# define STR_REDIR_IN		("<")
# define STR_REDIR_OUT		(">")
# define STR_REDIR_HEREDOC	("<<")
# define STR_REDIR_APPEND	(">>")
# define STR_NEWLINE		("\n")

/* ----- Bonus Part ----- */

# define STR_LPAREN			("(")
# define STR_RPAREN			(")")
# define STR_AND_IF			("&&")
# define STR_OR_IF			("||")

typedef enum e_token_type
{
	TK_EOF,					// 토큰 리스트 끝을 나타냄
	TK_NEWLINE,				// "\n"
	TK_BLANK,				// " ", "\t"
	TK_WORD,				// 공백, 탭으로 구별되는 일반 문자열
	TK_WORD_WITH_SQUOTE,	// 작은 따옴표로 묶여있는 문자열
	TK_WORD_WITH_DQUOTE,	// 큰 따옴표로 묶여있는 문자열
	TK_ASSIGN_WORD,			// "=" 을 포함하는 문자열
	TK_SEMICOLON,			// ";"
	TK_PIPE,				// "|"
	TK_PIPE_ERR,				// "|"
	TK_AMPERSAND,			// "&"
	TK_REDIR_IN,			// "<"
	TK_REDIR_OUT,			// ">"
	TK_REDIR_HEREDOC,		// "<<"
	TK_REDIR_APPEND,		// ">>"
	
	/* ----- Bonus Part ----- */

	TK_LPAREN,				// "("
	TK_RPAREN,				// ")"
	TK_AND_IF,				// "&&"
	TK_OR_IF,				// "||"
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	void			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;


/* --- tokenizer.c --- */

t_token			*tokenize_input(char *line);


/* --- token_create.c --- */

t_token 		*create_empty_token(void);
t_token			*create_token(char *str);


/* --- token_create_utils.c --- */

t_token_type	get_token_type(char *str);
void			set_token_value_from_str(t_token *target, char *str);
void			set_token_value_from_type(t_token *target);


/* --- token_lst_utils.c --- */

t_status		clear_token_lst(t_token **lst);
t_status		append_token_to_lst(t_token **lst, t_token *token);
t_status		remove_token_from_lst(t_token *lst, t_token *target);


/* --- token_word_utils.c --- */

char			*extract_squote_word(char *str);
char			*extract_dquote_word(char *str);
char			*extract_normal_word(char *str);


/* --- token_utils.c --- */

void			print_token(t_token *token);

#endif

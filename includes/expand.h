/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:22:16 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "libft.h"
# include "tokenizer.h"

# define MASK_QUOTED	'1'
# define MASK_UNQUOTED	'0'
# define MASK_IFS		'2'

# define PARAM_IDX		0
# define VALUE_IDX		1

# define EXP_TOKEN_LEN	0
# define PARAM_LEN		1
# define VALUE_LEN		2
# define LEN_SUM		3
# define SUFFIX_LEN		4

typedef struct s_exp_token
{
	char				*value;
	char				*qmask;
	struct s_exp_token	*prev;
	struct s_exp_token	*next;
}	t_exp_token;

/* --- expand_param.c --- */

t_exp_token	*expand_param(t_exp_token *exp_token);

/* --- expand_tilde.c --- */

t_exp_token	*expand_tilde(t_exp_token *exp_token);
char		*extract_tilde_with_username(char *value);

/* --- expand_tilde_utils.c --- */

char		*get_homedir(char *username);
t_boolean	check_homedir(char *username);
char		*get_home_prefix(void);

/* --- expand_token.c --- */

void		expand_heredoc_target(t_token **token);
t_token		*expand_token(t_token *token);

/* --- expand_token_create.c --- */

t_exp_token	*create_empty_exp_token(void);
t_exp_token	*create_exp_token(char *value);

/* --- expand_utils.c --- */

void		replace_exp(
				t_exp_token *exp_token,
				size_t *idx,
				char **env_pair);
void		quote_removal(void *content);
void		remove_exp_token(void *content);

/* --- masking_utils.c --- */

char		*masking_token(const char *value);
char		*expand_mask(
				char *quote_mask,
				size_t pos,
				char *before,
				char *after);

/* --- split_field.c --- */

void		split_field(
				t_list **exp_list,
				t_exp_token *exp_token);

#endif

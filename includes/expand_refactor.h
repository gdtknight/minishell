/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_refactor.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/24 22:42:53 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_REFACTOR_H
# define EXPAND_REFACTOR_H

# include "libft.h"
# include "tokenizer.h"

# define MASK_QUOTED	'1'
# define MASK_UNQUOTED	'0'
# define MASK_IFS		'2'

# define PARAM_IDX		0
# define VALUE_IDX		1

typedef enum e_state_idx
{
	IN_SQUOTE = 0,
	IN_DQUOTE = 1,
	IN_ESCAPE = 2,
}	t_state_idx;

typedef struct s_exp_token
{
	char				*value;
	char				*qmask;
	struct s_exp_token	*prev;
	struct s_exp_token	*next;
}	t_exp_token;

/* --- expand_token_refactor.c --- */

t_token	*expand_token_refactor(t_token *token);
void	expand_heredoc_target(t_token **token);

/* --- expand_tilde_refactor.c --- */

t_exp_token	*expand_tilde_refactor(t_exp_token *exp_token);
char		*extract_tilde_with_username(char *value);

/* --- expand_param_refactor.c --- */

t_exp_token	*expand_param_refactor(t_exp_token *exp_token);

/* --- split_field_refactor.c --- */

void		split_field_refactor(t_list **exp_list, t_exp_token *exp_token);

/* --- expand_token_create.c --- */

t_exp_token	*create_empty_exp_token(void);
t_exp_token	*create_exp_token(char *value);

/* --- masking_utils.c --- */

char		*masking_token(const char *value);
char		*expand_mask(
				char *quote_mask,
				size_t pos,
				char *before,
				char *after);

#endif

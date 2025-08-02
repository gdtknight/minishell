/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/02 21:01:46 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "tokenizer.h"

/* --- expander.c --- */

void	expand_tk_lst(t_token **tk_lst);
t_token	*expand_token(t_token *token);

/* --- expander_tilde.c --- */

t_token	*expand_tilde(t_token *token);

/* --- expander_envp.c --- */

t_token	*expand_envp(t_token *token);


t_token	*field_split(t_token *token);
t_token	*pathname_expansion(t_token *token);
t_token	*remove_quote(t_token *token);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 23:13:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "tokenizer.h"

/* --- expand.c --- */

t_token		*expand_token(t_token *token);

/* --- expand_envp.c --- */

char		*expand_envp(char *value);

/* --- expand_tilde.c --- */

char		*expand_tilde(char *value);

/* --- expand_utils.c --- */

char		*replace_param(char *str, char **new_value);
t_boolean	is_dollar_sign(char c);

#endif

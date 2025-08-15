/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:26:39 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:39:25 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "flag.h"
#include "tokenizer.h"
#include "utils.h"
#include "expand.h"

t_token	*expand_token(t_token *token)
{
	t_token	*new_token;
	char	*value;

	if (!token || token->type != TK_WORD)
		return (token);
	value = token->value;
	if (*value == '~')
	{
		value = expand_tilde(token->value);
		free(token->value);
		token->value = value;
	}
	if (find_next_delim(value, is_dollar_sign, C_BACKSLASH | C_SQUOTE))
	{
		value = expand_envp(token->value);
		free(token->value);
		token->value = value;
	}
	new_token = token;
	return (new_token);
}

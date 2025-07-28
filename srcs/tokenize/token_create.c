/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:44:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 14:44:54 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "tokenizer.h"

t_token	*create_empty_token(void)
{
	t_token	*token;

	token = (t_token *) malloc(sizeof(t_token));
	if (errno == ENOMEM)
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	token->type = TK_EOF;
	token->value = NULL;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*create_token(char *str)
{
	t_token	*new_token;

	new_token = create_empty_token();
	new_token->type = get_token_type(str);
	if (new_token->type == TK_BLANK)
	{
		free(new_token);
		return (NULL);
	}
	if (new_token->type == TK_WORD
		|| new_token->type == TK_WORD_WITH_SQUOTE
		|| new_token->type == TK_WORD_WITH_DQUOTE)
		set_token_value_from_str(new_token, str);
	else
		set_token_value_from_type(new_token);
	return (new_token);
}

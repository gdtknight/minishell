/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validate_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:02:50 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/19 03:24:04 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "def.h"

#include "tokenizer.h"

#include "debug.h"

t_boolean	is_valid_sequence(t_token *token_lst)
{
	t_boolean	in_parenthesis;

	in_parenthesis = FALSE;
	if (!token_lst)
		return (FALSE);
	if (token_lst->type == TK_EOF
		|| token_lst->type == TK_AND_IF || token_lst->type == TK_OR_IF
		|| token_lst->type == TK_PIPE || token_lst->type == TK_PIPE_ERR
		|| token_lst->type == TK_AMPERSAND)
		return (FALSE);
	while (token_lst)
	{
		if (token_lst->type == TK_LPAREN)
		{
			if (in_parenthesis)
				return (FALSE);
			else
				in_parenthesis = TRUE;
		}
		if (token_lst->type == TK_RPAREN && !in_parenthesis)
			return (FALSE);
		if (is_io_token(token_lst) && !is_word_token(token_lst->next))
		{
			debug("io (or op) token \'%s\' with none word token \'%s\'", \
		 		(char *)token_lst->value, (char *)token_lst->next->value);
			return (FALSE);
		}
		token_lst = token_lst->next;
	}
	if (in_parenthesis)
		return (FALSE);
	return (TRUE);
}

t_boolean	is_io_token(t_token *token)
{
	if (!token)
		return (FALSE);
	return (token->type == TK_REDIR_IN
		|| token->type == TK_REDIR_OUT
		|| token->type == TK_REDIR_APPEND
		|| token->type == TK_REDIR_HEREDOC);
}

t_boolean	is_op_token(t_token *token)
{
	if (!token)
		return (FALSE);
	return (token->type == TK_AND_IF
		|| token->type == TK_OR_IF
		|| token->type == TK_SEMICOLON
		|| token->type == TK_AMPERSAND
		|| token->type == TK_PIPE
		|| token->type == TK_PIPE_ERR
		|| token->type == TK_OR_IF);
}

t_boolean	is_word_token(t_token *token)
{
	if(!token)
		return (FALSE);
	return (token->type == TK_WORD);
}

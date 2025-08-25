/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validatation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:02:50 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/26 01:30:36 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer_validation.c
 * @brief Functions for validating sequences of tokens.
 *
 * Provides utilities to check whether a token sequence forms a valid
 * command according to shell syntax rules.
 */

#include "def.h"
#include "tokenizer.h"

static t_boolean	is_valid_token(t_token *token, t_boolean *in_parenthesis);

/**
 * @brief Check whether a sequence of tokens is valid.
 *
 * Iterates through the token list and verifies that:
 * - The first token is not an operator or EOF.
 * - Parentheses are properly opened and closed.
 * - Redirection tokens are followed by a word token.
 *
 * @param token_lst Head of the token list.
 * @return t_boolean TRUE if the sequence is valid, FALSE otherwise.
 */
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
		if (!is_valid_token(token_lst, &in_parenthesis))
			return (FALSE);
		token_lst = token_lst->next;
	}
	if (in_parenthesis)
		return (FALSE);
	return (TRUE);
}

/**
 * @brief Check if a token is a redirection operator.
 *
 * Supported types: TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_APPEND, TK_REDIR_HEREDOC.
 *
 * @param token Token to check.
 * @return t_boolean TRUE if it is an IO token, FALSE otherwise.
 */
t_boolean	is_io_token(t_token *token)
{
	if (!token)
		return (FALSE);
	return (token->type == TK_REDIR_IN
		|| token->type == TK_REDIR_OUT
		|| token->type == TK_REDIR_APPEND
		|| token->type == TK_REDIR_HEREDOC);
}

/**
 * @brief Check if a token is an operator.
 *
 * Supported types: TK_AND_IF, TK_OR_IF, TK_SEMICOLON, TK_AMPERSAND,
 * TK_PIPE, TK_PIPE_ERR.
 *
 * @param token Token to check.
 * @return t_boolean TRUE if it is an operator token, FALSE otherwise.
 */
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

/**
 * @brief Check if a token is a word.
 *
 * Supported type: TK_WORD.
 *
 * @param token Token to check.
 * @return t_boolean TRUE if it is a word token, FALSE otherwise.
 */
t_boolean	is_word_token(t_token *token)
{
	if (!token)
		return (FALSE);
	return (token->type == TK_WORD);
}

/**
 * @brief Validate a single token within a sequence.
 *
 * Performs checks for:
 * - Parentheses balance.
 * - IO token followed by a word token.
 *
 * @param token Current token to validate.
 * @param in_parenthesis Pointer to boolean tracking open parentheses.
 * @return t_boolean TRUE if the token is valid, FALSE otherwise.
 *
 * @note Memory management is not affected;
 *       this function only checks token rules.
 */
static t_boolean	is_valid_token(t_token *token, t_boolean *in_parenthesis)
{
	if (token->type == TK_LPAREN)
	{
		if (*in_parenthesis)
			return (FALSE);
		else
			*in_parenthesis = TRUE;
	}
	if (token->type == TK_RPAREN)
	{
		if (!*in_parenthesis)
			return (FALSE);
		else
			*in_parenthesis = FALSE;
	}
	if (is_io_token(token) && !is_word_token(token->next))
		return (FALSE);
	return (TRUE);
}

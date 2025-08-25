/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:24:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:08:54 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file token_printer.c
 * @brief Utilities for printing token types and values for debugging.
 *
 * Provides functions to print token information depending on the token type.
 * Supports WORD, ASSIGN_WORD, single-character tokens, multi-character
 * operators, and redirection tokens.
 */

#include <stdio.h>
#include "tokenizer.h"

static void	print_single_char_token(t_token *token);
static void	print_redir_token(t_token *token);
static void	print_str_token(t_token *token);

/**
 * @brief Check if a token matches a given type.
 *
 * @param token   Token to check.
 * @param tk_type Token type to compare against.
 * @return t_boolean TRUE if token type matches, FALSE otherwise.
 */
t_boolean	match(t_token *token, t_token_type tk_type)
{
	if (!token)
		return (FALSE);
	return (token->type == tk_type);
}

/**
 * @brief Print the token's type and value.
 *
 * Delegates printing to the appropriate helper function depending on token type:
 * - TK_WORD, TK_ASSIGN_WORD → print_str_token()
 * - TK_PIPE_ERR, TK_OR_IF, TK_AND_IF → printed directly
 * - Single-character tokens (EOF, &, \n, blank, ;, |, (, ))
 *   → print_single_char_token()
 * - Redirection tokens (<, >, <<, >>) → print_redir_token()
 *
 * @param token Token to print.
 */
void	print_token(t_token *token)
{
	if (match(token, TK_WORD) || match(token, TK_ASSIGN_WORD))
		print_str_token(token);
	if (match(token, TK_PIPE_ERR))
		printf("type - %s, value - %s\n", STR_PIPE_ERR, (char *)(token->value));
	if (match(token, TK_OR_IF))
		printf("type - %s, value - %s\n", STR_OR_IF, (char *)(token->value));
	if (match(token, TK_AND_IF))
		printf("type - %s, value - %s\n", STR_AND_IF, (char *)(token->value));
	if (match(token, TK_EOF) || match(token, TK_AMPERSAND)
		|| match(token, TK_NEWLINE) || match(token, TK_BLANK)
		|| match(token, TK_SEMICOLON) || match(token, TK_PIPE)
		|| match(token, TK_LPAREN) || match(token, TK_RPAREN))
		print_single_char_token(token);
	if (match(token, TK_REDIR_IN)
		|| match(token, TK_REDIR_OUT)
		|| match(token, TK_REDIR_HEREDOC)
		|| match(token, TK_REDIR_APPEND))
		print_redir_token(token);
}

/**
 * @brief Print single-character token types.
 *
 * Supported types: TK_EOF, TK_NEWLINE, TK_BLANK, TK_SEMICOLON, TK_AMPERSAND,
 * TK_PIPE, TK_LPAREN, TK_RPAREN.
 *
 * @param token Token to print.
 */
static void	print_single_char_token(t_token *token)
{
	if (match(token, TK_EOF))
		printf("type - %s, value - %s\n", STR_EOF, (char *)(token->value));
	if (match(token, TK_NEWLINE))
		printf("type - %s, value - %s\n", STR_NEWLINE, (char *)(token->value));
	if (match(token, TK_BLANK))
		printf("type - %s, value - %s\n", STR_BLANK, (char *)(token->value));
	if (match(token, TK_SEMICOLON))
		printf("type - %s, value - %s\n", STR_SEMICOLON, (char *)(token->value));
	if (match(token, TK_AMPERSAND))
		printf("type - %s, value - %s\n", STR_AMPERSAND, (char *)(token->value));
	if (match(token, TK_PIPE))
		printf("type - %s, value - %s\n", STR_PIPE, (char *)(token->value));
	if (match(token, TK_LPAREN))
		printf("type - %s, value - %s\n", STR_LPAREN, (char *)(token->value));
	if (match(token, TK_RPAREN))
		printf("type - %s, value - %s\n", STR_RPAREN, (char *)(token->value));
}

/**
 * @brief Print redirection token types.
 *
 * Supported types: TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_HEREDOC, TK_REDIR_APPEND.
 *
 * @param token Token to print.
 */
static void	print_redir_token(t_token *token)
{
	if (match(token, TK_REDIR_IN))
		printf("type - %s, value - %s\n", STR_REDIR_IN, (char *)(token->value));
	if (match(token, TK_REDIR_OUT))
		printf("type - %s, value - %s\n", STR_REDIR_OUT, (char *)(token->value));
	if (match(token, TK_REDIR_HEREDOC))
		printf("type - %s, value - %s\n", \
			STR_REDIR_HEREDOC, (char *)(token->value));
	if (match(token, TK_REDIR_APPEND))
		printf("type - %s, value - %s\n", \
			STR_REDIR_APPEND, (char *)(token->value));
}

/**
 * @brief Print string-value token types.
 *
 * Supported types: TK_WORD, TK_ASSIGN_WORD.
 *
 * @param token Token to print.
 */
static void	print_str_token(t_token *token)
{
	if (match(token, TK_WORD))
		printf("type - %s, value - %s\n", "TK_WORD", (char *)(token->value));
	if (match(token, TK_ASSIGN_WORD))
		printf("type - %s, value - %s\n", \
			"TK_ASSIGN_WORD", (char *)(token->value));
}

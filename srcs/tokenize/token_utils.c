/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:24:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 21:12:28 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "tokenizer.h"

static void	print_single_char_token(t_token *token);
static void	print_redir_token(t_token *token);
static void	print_str_token(t_token *token);

/**
 * @brief 토큰 타입이 지정한 타입과 일치하는지 비교한다.
 *
 * @param token   비교할 토큰
 * @param tk_type 비교 대상 토큰 타입
 * @return t_boolean TRUE(일치), FALSE(불일치)
 */
t_boolean	match(t_token	*token, t_token_type tk_type)
{
	return (token->type == tk_type);
}

/**
 * @brief 토큰의 타입과 값을 포맷에 맞춰 출력한다.
 *
 * 토큰 타입에 따라 적절한 출력 함수로 위임:
 * - TK_WORD, TK_ASSIGN_WORD → print_str_token()
 * - TK_PIPE_ERR, TK_OR_IF, TK_AND_IF → 직접 printf()
 * - 단일 문자 토큰(TK_EOF, TK_AMPERSAND, TK_NEWLINE, TK_BLANK, TK_SEMICOLON,
 *   TK_PIPE, TK_LPAREN, TK_RPAREN) → print_single_char_token()
 * - 리다이렉션 토큰(TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_HEREDOC,
 *   TK_REDIR_APPEND) → print_redir_token()
 *
 * @param token 출력할 토큰
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
 * @brief 단일 문자로 표현되는 토큰 타입을 출력한다.
 *
 * TK_EOF, TK_NEWLINE, TK_BLANK, TK_SEMICOLON, TK_AMPERSAND,
 * TK_PIPE, TK_LPAREN, TK_RPAREN 타입에 대해 문자열 상수와 값을 출력한다.
 *
 * @param token 출력할 토큰
 */
static void	print_single_char_token(t_token *token)
{
	if (match(token, TK_EOF))
		printf("type - %s, value - %s\n", \
			STR_EOF, (char *)(token->value));
	if (match(token, TK_NEWLINE))
		printf("type - %s, value - %s\n", \
			STR_NEWLINE, (char *)(token->value));
	if (match(token, TK_BLANK))
		printf("type - %s, value - %s\n", \
			STR_BLANK, (char *)(token->value));
	if (match(token, TK_SEMICOLON))
		printf("type - %s, value - %s\n", \
			STR_SEMICOLON, (char *)(token->value));
	if (match(token, TK_AMPERSAND))
		printf("type - %s, value - %s\n", \
			STR_AMPERSAND, (char *)(token->value));
	if (match(token, TK_PIPE))
		printf("type - %s, value - %s\n", \
			STR_PIPE, (char *)(token->value));
	if (match(token, TK_LPAREN))
		printf("type - %s, value - %s\n", \
			STR_LPAREN, (char *)(token->value));
	if (match(token, TK_RPAREN))
		printf("type - %s, value - %s\n", \
			STR_RPAREN, (char *)(token->value));
}

/**
 * @brief 리다이렉션 토큰 타입을 출력한다.
 *
 * TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_HEREDOC, TK_REDIR_APPEND 타입에 대해
 * 문자열 상수와 값을 출력한다.
 *
 * @param token 출력할 토큰
 */
static void	print_redir_token(t_token *token)
{
	if (match(token, TK_REDIR_IN))
		printf("type - %s, value - %s\n", \
			STR_REDIR_IN, (char *)(token->value));
	if (match(token, TK_REDIR_OUT))
		printf("type - %s, value - %s\n", \
			STR_REDIR_OUT, (char *)(token->value));
	if (match(token, TK_REDIR_HEREDOC))
		printf("type - %s, value - %s\n", \
			STR_REDIR_HEREDOC, (char *)(token->value));
	if (match(token, TK_REDIR_APPEND))
		printf("type - %s, value - %s\n", \
			STR_REDIR_APPEND, (char *)(token->value));
}

/**
 * @brief 문자열 값을 가지는 토큰 타입을 출력한다.
 *
 * TK_WORD, TK_ASSIGN_WORD 타입에 대해 타입명과 문자열 값을 출력한다.
 *
 * @param token 출력할 토큰
 */
static void	print_str_token(t_token *token)
{
	if (match(token, TK_WORD))
		printf("type - %s, value - %s\n", \
			"TK_WORD", (char *)(token->value));
	if (match(token, TK_ASSIGN_WORD))
		printf("type - %s, value - %s\n", \
			"TK_ASSIGN_WORD", (char *)(token->value));
}

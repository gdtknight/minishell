/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:24:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/28 13:58:29 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "tokenizer.h"

static void	print_single_char_token(t_token *token);

t_boolean	match(t_token	*token, t_token_type tk_type)
{
	return (token->type ==  tk_type);
}

void	print_token(t_token *token)
{
	if (match(token, TK_WORD))
		printf("type - %s, value - %s\n", "TK_WORD", (char *) (token->value));
	if (match(token, TK_WORD_WITH_SQUOTE))
		printf("type - %s, value - %s\n", "TK_WORD_WITH_SQUOTE", (char *) (token->value));
	if (match(token, TK_WORD_WITH_DQUOTE))
		printf("type - %s, value - %s\n", "TK_WORD_WITH_DQUOTE", (char *) (token->value));
	if (match(token, TK_ASSIGN_WORD))
		printf("type - %s, value - %s\n", "TK_ASSIGN_WORD", (char *) (token->value));
	if (match(token, TK_PIPE_ERR))
		printf("type - %s, value - %s\n", STR_PIPE_ERR, (char *) (token->value));
	if (match(token, TK_AMPERSAND))
		printf("type - %s, value - %s\n", STR_AND_IF, (char *) (token->value));
	if (match(token, TK_OR_IF))
		printf("type - %s, value - %s\n", STR_OR_IF, (char *) (token->value));
	if (match(token, TK_AND_IF))
		printf("type - %s, value - %s\n", STR_PIPE, (char *) (token->value));
	if (match(token, TK_EOF) || match(token, TK_NEWLINE) || match(token, TK_BLANK)
		|| match(token, TK_SEMICOLON) || match(token, TK_PIPE)
		|| match(token, TK_LPAREN) || match(token, TK_RPAREN)
		|| match(token, TK_REDIR_IN) || match(token, TK_REDIR_OUT)
		|| match(token, TK_REDIR_HEREDOC) || match(token, TK_REDIR_APPEND))
		print_single_char_token(token);
}

static void	print_single_char_token(t_token *token)
{
	if (match(token, TK_EOF))
		printf("type - %s, value - %s\n", STR_EOF, (char *) (token->value));
	if (match(token, TK_NEWLINE))
		printf("type - %s, value - %s\n", STR_NEWLINE, (char *) (token->value));
	if (match(token, TK_BLANK))
		printf("type - %s, value - %s\n", STR_BLANK, (char *) (token->value));
	if (match(token, TK_SEMICOLON))
		printf("type - %s, value - %s\n", STR_SEMICOLON, (char *) (token->value));
	if (match(token, TK_PIPE))
		printf("type - %s, value - %s\n", STR_AMPERSAND, (char *) (token->value));
	if (match(token, TK_REDIR_IN))
		printf("type - %s, value - %s\n", STR_REDIR_IN, (char *) (token->value));
	if (match(token, TK_REDIR_OUT))
		printf("type - %s, value - %s\n", STR_REDIR_OUT, (char *) (token->value));
	if (match(token, TK_REDIR_HEREDOC))
		printf("type - %s, value - %s\n", STR_REDIR_HEREDOC, (char *) (token->value));
	if (match(token, TK_REDIR_APPEND))
		printf("type - %s, value - %s\n", STR_REDIR_APPEND, (char *) (token->value));
	if (match(token, TK_LPAREN))
		printf("type - %s, value - %s\n", STR_LPAREN, (char *) (token->value));
	if (match(token, TK_RPAREN))
		printf("type - %s, value - %s\n", STR_RPAREN, (char *) (token->value));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:27:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:11:26 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer.c
 * @brief Tokenizes input lines into a linked list of t_token structures.
 *
 * Provides functions to convert a command line string into tokens,
 * handle expansions (variables, heredoc targets), and detect ambiguous
 * redirections.
 */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"
#include "libft.h"
#include "expand.h"
#include "tokenizer.h"

static char			*tokenize(
						char *cursor,
						t_token **token_lst,
						t_token **new_token);
static t_boolean	check_ambigous(t_token **token_lst, t_token *token);

/**
 * @brief Tokenize an input line into a linked list of tokens.
 *
 * Iterates through the input line, generating tokens for each segment,
 * performing expansions for word tokens and heredoc targets, and
 * appending them to a token list.
 *
 * @param line Input command line string.
 * @return t_token* Head of the resulting token list (terminated with TK_EOF).
 *                  Returns NULL if tokenization or ambiguous redirection fails.
 */
t_token	*tokenize_input(char *line)
{
	t_token	*token_lst;
	t_token	*new_token;
	char	*cursor;

	if (!line)
		return (NULL);
	cursor = line;
	token_lst = NULL;
	new_token = NULL;
	while (*cursor)
	{
		cursor = tokenize(cursor, &token_lst, &new_token);
		if (!cursor)
			return (NULL);
	}
	new_token = create_token(NULL);
	new_token->type = TK_EOF;
	new_token->value = NULL;
	append_token_to_lst(&token_lst, new_token);
	return (token_lst);
}

/**
 * @brief Tokenize a single segment of the input line.
 *
 * Creates a new token, performs expansions if needed, checks for
 * ambiguous redirections, and appends the token to the token list.
 *
 * @param cursor Current position in the input line.
 * @param token_lst Pointer to the head of the token list.
 * @param new_token Pointer to store the newly created token.
 * @return char* Updated cursor position after consuming the token.
 *               Returns NULL on failure (e.g., ambiguous redirect).
 */
static char	*tokenize(char *cursor, t_token **token_lst, t_token **new_token)
{
	t_token	*last_token;

	*new_token = create_token(cursor);
	if (!*new_token)
		return (cursor + 1);
	cursor += (ft_strlen((char *)((*new_token)->value)));
	if (check_ambigous(token_lst, *new_token))
	{
		clear_token_lst(token_lst);
		return (NULL);
	}
	last_token = get_last_token(token_lst);
	if (last_token && last_token->type == TK_REDIR_HEREDOC)
		expand_heredoc_target(new_token);
	else if ((*new_token)->type == TK_WORD)
	{
		last_token = *new_token;
		*new_token = expand_token(last_token);
		free(last_token->value);
		free(last_token);
	}
	if (*new_token)
		append_token_to_lst(token_lst, *new_token);
	return (cursor);
}

/**
 * @brief Check for ambiguous redirection after expansion.
 *
 * If the last token is an IO redirection, this function expands the
 * current word token and verifies that it does not result in
 * multiple targets (ambiguous redirect).
 *
 * @param token_lst Pointer to the head of the token list.
 * @param token Current token to check.
 * @return t_boolean TRUE if ambiguous, FALSE otherwise.
 *
 * @note Prints an error message to STDERR if an ambiguous redirect is detected.
 */
static t_boolean	check_ambigous(t_token **token_lst, t_token *token)
{
	t_token	*last_token;
	t_token	*expanded;

	last_token = get_last_token(token_lst);
	if (!token || !last_token || !is_io_token(last_token)
		|| (token->type != TK_WORD))
		return (FALSE);
	expanded = expand_token(token);
	if (!expanded)
		return (FALSE);
	if (!expanded->next || !(expanded->next->type == TK_WORD))
	{
		clear_token_lst(&expanded);
		return (FALSE);
	}
	ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putendl_fd(": ambigous redirect", STDERR_FILENO);
	clear_token_lst(&expanded);
	return (TRUE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_factory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:44:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/28 23:09:12 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file token_factory.c
 * @brief Token creation utilities for the tokenizer.
 *
 * This file provides functions to create tokens from strings or
 * initialize empty tokens. It handles type detection, value assignment,
 * and ensures consistent initialization of the `t_token` structure.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "libft.h"
#include "tokenizer.h"

/**
 * @brief Create an empty token structure.
 *
 * Dynamically allocates memory for a `t_token` structure, initializes
 * it with default values (type = TK_EOF, value = NULL, no links), and
 * returns a pointer to it.
 *
 * @return Pointer to the initialized empty token.
 *
 * @note
 * - If memory allocation fails with ENOMEM, prints an error message
 *   using `perror()` and terminates the program with `exit(EXIT_FAILURE)`.
 * - The returned token must be freed by the caller (directly or via
 *   a higher-level token cleanup function).
 */
t_token	*create_empty_token(void)
{
	t_token	*token;

	token = (t_token *) ft_calloc(sizeof(t_token), 1);
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

/**
 * @brief Create a token from a given string.
 *
 * Analyzes the input string, determines the token type, and assigns
 * its value accordingly. Blank tokens (type TK_BLANK) are discarded.
 *
 * @param str The source string used for token creation.
 * @return Pointer to the created token, or NULL if the token type is TK_BLANK.
 *
 * @note
 * - Internally calls `create_empty_token()` for initialization.
 * - Token type is determined via `get_token_type()`.
 * - If the type is TK_WORD, the value is set from the string using
 *   `set_token_value_from_str()`. For other types,
 *   `set_token_value_from_type()` is used.
 * - The returned token must eventually be freed (e.g., with `free_token()`).
 */
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
	if (new_token->type == TK_WORD)
		set_token_value_from_str(new_token, str);
	else
		set_token_value_from_type(new_token);
	return (new_token);
}

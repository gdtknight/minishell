/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:40:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:24:45 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer_value.c
 * @brief Functions to determine token types and assign token values.
 *
 * This module provides utilities to:
 * 1. Identify the token type at a given position in the input string.
 *    - Handles multi-character operators: <<, >>, &&, ||
 *    - Handles single-character meta-characters: ; & | \n < > ( )
 *    - Handles blanks and words
 * 2. Set token values based on type:
 *    - TK_WORD tokens extract substring up to the next delimiter.
 *    - Operator tokens copy predefined string constants.
 *
 * @note Per project convention, switch statements are not used; all type
 *       checks are done using conditional statements.
 *
 * @see get_token_type(), set_token_value_from_str(),
 *      set_token_value_from_type(), set_token_value_from_single_char()
 */

#include "libft.h"
#include "flag.h"
#include "utils.h"
#include "tokenizer.h"

static t_token_type	get_single_char_token_type(char *str);
static void			set_token_value_from_single_char(t_token *target);

/**
 * @brief Determine the token type for the current position in the string.
 *
 * Multi-character operators (<<, >>, &&, ||) are recognized first.
 * Single-character meta characters (;, &, |, \n, <, >, (, )) and
 * whitespace (space or tab) are recognized accordingly.
 * All other sequences are classified as TK_WORD.
 *
 * @param str Pointer to the current position in the input string.
 * @return t_token_type Detected token type.
 */
t_token_type	get_token_type(char *str)
{
	if (!str || !*str)
		return (TK_EOF);
	if (ft_strncmp(str, STR_REDIR_HEREDOC, ft_strlen(STR_REDIR_HEREDOC)) == 0)
		return (TK_REDIR_HEREDOC);
	if (ft_strncmp(str, STR_REDIR_APPEND, ft_strlen(STR_REDIR_APPEND)) == 0)
		return (TK_REDIR_APPEND);
	if (ft_strncmp(str, STR_AND_IF, ft_strlen(STR_AND_IF)) == 0)
		return (TK_AND_IF);
	if (ft_strncmp(str, STR_OR_IF, ft_strlen(STR_OR_IF)) == 0)
		return (TK_OR_IF);
	if (*str == ' ' || *str == '\t')
		return (TK_BLANK);
	if (*str == ';' || *str == '&' || *str == '|' || *str == '\n'
		|| *str == '<' || *str == '>' || *str == '(' || *str == ')')
		return (get_single_char_token_type(str));
	return (TK_WORD);
}

/**
 * @brief Map single-character meta characters to their token type.
 *
 * Handles the following characters: ; & | \n < > ( )
 *
 * @param str Pointer to the single-character meta character.
 * @return t_token_type Corresponding token type.
 */
static t_token_type	get_single_char_token_type(char *str)
{
	if (*str == ';')
		return (TK_SEMICOLON);
	if (*str == '&')
		return (TK_AMPERSAND);
	if (*str == '|')
		return (TK_PIPE);
	if (*str == '\n')
		return (TK_NEWLINE);
	if (*str == '<')
		return (TK_REDIR_IN);
	if (*str == '>')
		return (TK_REDIR_OUT);
	if (*str == '(')
		return (TK_LPAREN);
	else
		return (TK_RPAREN);
}

/**
 * @brief Set the token value for a TK_WORD token.
 *
 * Extracts the substring up to the next delimiter, considering
 * escape sequences, quotes, and other separators.
 *
 * @param target Token whose value will be set (must be TK_WORD).
 * @param str    Pointer to the input string at the current position.
 */
void	set_token_value_from_str(t_token *target, char *str)
{
	target->value = ft_substr(str, 0, \
		find_next_delim(str, is_tk_delim, \
			C_BACKSLASH | C_SQUOTE | C_DQUOTE) - str);
}

/**
 * @brief Set the token value based on its non-WORD type.
 *
 * Multi-character operators (&&, ||, <<, >>) are assigned string constants.
 * Single-character operators delegate to set_token_value_from_single_char().
 *
 * @param target Token whose value will be set (must NOT be TK_WORD).
 */
void	set_token_value_from_type(t_token *target)
{
	if (target->type == TK_AND_IF)
		target->value = ft_strdup(STR_AND_IF);
	if (target->type == TK_OR_IF)
		target->value = ft_strdup(STR_OR_IF);
	if (target->type == TK_REDIR_HEREDOC)
		target->value = ft_strdup(STR_REDIR_HEREDOC);
	if (target->type == TK_REDIR_APPEND)
		target->value = ft_strdup(STR_REDIR_APPEND);
	if (target->type == TK_SEMICOLON
		|| target->type == TK_PIPE
		|| target->type == TK_AMPERSAND
		|| target->type == TK_REDIR_IN
		|| target->type == TK_REDIR_OUT
		|| target->type == TK_LPAREN
		|| target->type == TK_RPAREN
		|| target->type == TK_NEWLINE)
		set_token_value_from_single_char(target);
}

/**
 * @brief Set the token value for single-character operators.
 *
 * Duplicates the corresponding string constant for the token type:
 * ;, &, |, \n, <, >, (, )
 *
 * @param target Token whose value will be set.
 */
static void	set_token_value_from_single_char(t_token *target)
{
	if (target->type == TK_SEMICOLON)
		target->value = ft_strdup(STR_SEMICOLON);
	if (target->type == TK_AMPERSAND)
		target->value = ft_strdup(STR_AMPERSAND);
	if (target->type == TK_PIPE)
		target->value = ft_strdup(STR_PIPE);
	if (target->type == TK_NEWLINE)
		target->value = ft_strdup(STR_NEWLINE);
	if (target->type == TK_REDIR_IN)
		target->value = ft_strdup(STR_REDIR_IN);
	if (target->type == TK_REDIR_OUT)
		target->value = ft_strdup(STR_REDIR_OUT);
	if (target->type == TK_LPAREN)
		target->value = ft_strdup(STR_LPAREN);
	if (target->type == TK_RPAREN)
		target->value = ft_strdup(STR_RPAREN);
}

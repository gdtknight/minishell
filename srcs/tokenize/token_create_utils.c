/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:40:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:03:59 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "flag.h"
#include "utils.h"

#include "tokenizer.h"

static t_token_type	get_single_char_token_type(char *str);
static void			set_token_value_from_single_char(t_token *target);

/**
 * @brief 주어진 위치의 문자열을 분석해 토큰 타입을 판별한다.
 *
 * 2글자 연산자(<<, >>, &&, ||)를 우선 인식하고, 공백/탭, 1글자 메타문자
 * (;, &, |, \n, <, >, (, ))를 구분한다. 그 외는 일반 단어(TK_WORD)로 분류한다.
 *
 * @param str 현재 커서를 가리키는 문자열 포인터
 * @return t_token_type 판별된 토큰 타입
 * @retval TK_EOF      str가 NULL이거나 빈 문자열인 경우
 * @retval TK_REDIR_HEREDOC  문자열이 STR_REDIR_HEREDOC("<<")로 시작
 * @retval TK_REDIR_APPEND   문자열이 STR_REDIR_APPEND(">>")로 시작
 * @retval TK_AND_IF         문자열이 STR_AND_IF("&&")로 시작
 * @retval TK_OR_IF          문자열이 STR_OR_IF("||")로 시작
 * @retval TK_BLANK          선두 문자가 공백(' ') 또는 탭('\t')
 * @retval TK_SEMICOLON/TK_AMPERSAND/TK_PIPE/TK_NEWLINE/
 *         TK_REDIR_IN/TK_REDIR_OUT/TK_LPAREN/TK_RPAREN  해당 1글자 메타문자
 * @retval TK_WORD           위 어느 것도 아닌 일반 단어
 *
 * @note 내부적으로 1글자 메타문자는 get_single_char_token_type()에서 매핑한다.
 */
t_token_type	get_token_type(char *str)
{
	if (!str || !*str)
		return (TK_EOF);
	if (ft_strncmp(str, STR_REDIR_HEREDOC, \
				ft_strlen(STR_REDIR_HEREDOC)) == 0)
		return (TK_REDIR_HEREDOC);
	if (ft_strncmp(str, STR_REDIR_APPEND, \
				ft_strlen(STR_REDIR_APPEND)) == 0)
		return (TK_REDIR_APPEND);
	if (ft_strncmp(str, STR_AND_IF, \
				ft_strlen(STR_AND_IF)) == 0)
		return (TK_AND_IF);
	if (ft_strncmp(str, STR_OR_IF, \
				ft_strlen(STR_OR_IF)) == 0)
		return (TK_OR_IF);
	if (*str == ' ' || *str == '\t')
		return (TK_BLANK);
	if (*str == ';' || *str == '&' || *str == '|' || *str == '\n'
		|| *str == '<' || *str == '>' || *str == '(' || *str == ')')
		return (get_single_char_token_type(str));
	return (TK_WORD);
}

/**
 * @brief 단일 문자 메타문자를 대응하는 토큰 타입으로 변환한다.
 *
 * ';' '&' '|' '\n' '<' '>' '(' ')' 중 하나를 입력으로 받아
 * 각 문자에 해당하는 t_token_type을 반환한다.
 *
 * @param str 단일 메타문자로 시작하는 문자열 포인터
 * @return t_token_type 해당 문자의 토큰 타입
 *
 * @note 외부에 노출되지 않는 내부 유틸(static) 함수이다.
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
 * @brief Set the token value for a TK_WORD token from the original string.
 *
 * This function extracts the substring up to the next delimiter (considering
 * escape sequences, quotes, and separators) and stores it as the token value.
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
 * @brief Set the token value for non-WORD tokens based on their type.
 *
 * Multi-character operators (&&, ||, <<, >>) are set to their corresponding
 * string constants. Single-character operators are delegated to 
 * set_token_value_from_single_char().
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
 * This function duplicates the constant string corresponding to the token type:
 * ';', '&', '|', '\n', '<', '>', '(', ')'.
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

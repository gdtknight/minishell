/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:40:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/17 23:12:51 by yoshin           ###   ########.fr       */
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
 * @brief TK_WORD 타입 토큰의 value를 원본 문자열에서 추출해 설정한다.
 *
 * 인용부호, 이스케이프, 구분자 처리 정책에 따라 일반 단어 토큰의
 * 실제 문자열 값을 추출(extract_normal_word)하여 target->value에 저장한다.
 *
 * @param target 값이 설정될 토큰 포인터 (type == TK_WORD 여야 함)
 * @param str    원본 문자열(현재 커서)
 *
 * @note extract_normal_word()가 동적할당한 문자열을 target->value로 소유한다.
 *       이후 메모리 해제는 토큰 해제 시 호출자 책임이다.
 */
void	set_token_value_from_str(t_token *target, char *str)
{
	target->value = ft_substr(str, 0, \
		find_next_delim(str, is_tk_delim, \
				  C_BACKSLASH | C_SQUOTE | C_DQUOTE) - str);
}

/**
 * @brief 비-WORD 연산자 타입 토큰의 value를 타입 상수 문자열로 설정한다.
 *
 * AND_IF, OR_IF, HEREDOC, APPEND 등 멀티문자 연산자는 대응하는 STR_* 상수로,
 * 그 외 단일문자 메타문자는 내부 set_token_value_from_single_char()로 설정한다.
 *
 * @param target 값이 설정될 토큰 포인터 (type은 TK_WORD가 아니어야 함)
 *
 * @note 내부적으로 ft_strdup()으로 복제하여 target->value에 저장한다.
 *       메모리 해제는 토큰 해제 시 호출자 책임이다.
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
		|| target->type == TK_RPAREN)
		set_token_value_from_single_char(target);
}

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

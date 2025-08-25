/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:44:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 10:21:40 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "libft.h"

#include "tokenizer.h"

/**
 * @brief 비어 있는 토큰 구조체를 생성한다.
 *
 * `t_token` 구조체 메모리를 동적 할당하고, 기본값(TK_EOF, NULL 포인터, NULL 연결)을
 * 설정한 뒤 반환한다. 메모리 할당 실패 시 에러를 출력하고 프로그램을 종료한다.
 *
 * @return t_token* 초기화된 빈 토큰 구조체 포인터
 *
 * @note
 * - 메모리 부족(ENOMEM) 시 `perror()`로 에러 메시지를 출력하고 `exit(EXIT_FAILURE)`로 종료한다.
 * - 반환된 토큰은 후속 처리에서 타입과 값을 설정해야 한다.
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
 * @brief 주어진 문자열로부터 토큰을 생성한다.
 *
 * 문자열을 분석해 해당 토큰의 타입을 결정하고, 타입에 맞게 값을 설정한 뒤
 * `t_token` 구조체를 반환한다.
 *
 * @param str 토큰화를 위한 원본 문자열
 * @return t_token* 생성된 토큰 구조체 포인터
 * @retval NULL 토큰 타입이 TK_BLANK인 경우(공백 토큰은 생성하지 않음)
 *
 * @note
 * - 내부적으로 `create_empty_token()`을 호출하여 기본 토큰을 생성한다.
 * - 토큰 타입은 `get_token_type()`으로 결정된다.
 * - 타입이 TK_WORD이면 `set_token_value_from_str()`로 값을 설정하고,
 *   그 외에는 `set_token_value_from_type()`으로 설정한다.
 * - 반환된 토큰은 사용 후 `free_token()` 등으로 해제해야 한다.
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

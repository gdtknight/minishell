/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:29:00 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/18 01:45:06 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "ast.h"

/**
 * @brief <shell_input> 구문 규칙에 따라 구문 트리를 구성한다.
 *
 * <shell_input>  ::= <list> <newline_list>  
 * <newline_list> ::= <newline>  
 *                  | <newline_list> <newline>  
 * <newline>      ::= "\n"
 *
 * 입력 토큰 시퀀스를 기반으로 전체 명령어 입력을 구문 분석한다.
 * <list>를 먼저 처리한 뒤, 뒤따르는 모든 개행(Newline) 토큰들을 건너뛰고,
 * 입력의 끝(TK_EOF)이 아닌 경우 오류로 처리한다.
 *
 * @param tk_lst 현재 토큰 리스트 포인터의 주소.  
 *               함수 실행 중 토큰 리스트 포인터가 진행된다.
 * @return 성공 시 구문 트리 루트 노드 포인터.  
 *         입력이 유효하지 않으면 NULL을 반환한다.
 */
t_syntax_node	*parse_input(t_token **tk_lst)
{
	t_syntax_node	*input_node;

	input_node = list(tk_lst);
	while ((*tk_lst)->type == TK_NEWLINE)
		(*tk_lst) = (*tk_lst)->next;
	if ((*tk_lst)->type != TK_EOF)
	{
		printf("invalid input");
		return (NULL);
	}
	return (input_node);
}

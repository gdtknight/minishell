/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:27:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/17 18:18:02 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "tokenizer.h"
#include "debug.h"

t_token	*tokenize_input(char *line)
{
	t_token	*token_lst;
	t_token	*new_token;
	char	*cursor;

	cursor = line;
	token_lst = NULL;
	new_token = NULL;
	while (*cursor)
	{
		new_token = create_token(cursor);
		if (!new_token)
			cursor++;
		else
		{
			append_token_to_lst(&token_lst, new_token);
			if (new_token->type == TK_WORD_WITH_SQUOTE
				|| new_token->type == TK_WORD_WITH_DQUOTE)
				cursor += (ft_strlen((char *) (new_token->value) + 2));
			else
				cursor += (ft_strlen((char *) (new_token->value)));
		}
	}
	return (token_lst);
}

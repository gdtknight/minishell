/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:27:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 17:36:12 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "def.h"
#include "libft.h"

#include "tokenizer.h"

static t_boolean	is_dquoted_value(char *value);
static t_boolean	is_squoted_value(char *value);
static char			*tokenize(char *cursor, t_token **token_lst, t_token *new_token);

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
		cursor = tokenize(cursor, &token_lst, new_token);
	new_token = create_token(NULL);
	new_token->type = TK_EOF;
	new_token->value = NULL;
	append_token_to_lst(&token_lst, new_token);
	return (token_lst);
}

static char	*tokenize(char *cursor, t_token **token_lst, t_token *new_token)
{
	new_token = create_token(cursor);
	if (!new_token)
		return (cursor + 1);
	cursor += (ft_strlen((char *)(new_token->value)));
	if (is_squoted_value(new_token->value))
		new_token->type = TK_WORD_WITH_SQUOTE;
	if (is_dquoted_value(new_token->value))
		new_token->type = TK_WORD_WITH_DQUOTE;
	append_token_to_lst(token_lst, new_token);
	return (cursor);
}

static t_boolean	is_dquoted_value(char *value)
{
	char	m_flag;

	m_flag = 0;
	while (*value)
	{
		if (!(m_flag & C_BACKSLASH))
		{
			if (*value == '\\')
				m_flag ^= C_BACKSLASH;
			else if (*value == '\"')
				return (TRUE);
		}
		else
			m_flag ^= C_BACKSLASH;
		value++;
	}
	return (FALSE);
}

static t_boolean	is_squoted_value(char *value)
{
	char	m_flag;

	m_flag = 0;
	while (*value)
	{
		if (!(m_flag & C_BACKSLASH))
		{
			if (*value == '\\')
				m_flag ^= C_BACKSLASH;
			else if (*value == '\'')
				return (TRUE);
		}
		else
			m_flag ^= C_BACKSLASH;
		value++;
	}
	return (FALSE);
}

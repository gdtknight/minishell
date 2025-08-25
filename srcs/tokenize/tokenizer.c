/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:27:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:25:09 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static t_boolean	check_ambigous(
						t_token **token_lst,
						t_token *token);

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
	append_token_to_lst(token_lst, *new_token);
	return (cursor);
}

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
		return (FALSE);
	ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putendl_fd(": ambigous redirect", STDERR_FILENO);
	clear_token_lst(&expanded);
	return (TRUE);
}

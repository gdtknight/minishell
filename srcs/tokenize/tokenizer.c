/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:27:10 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 14:25:12 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"
#include "libft.h"

#include "expand.h"
#include "tokenizer.h"

#include "debug.h"

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
	t_token	*cur;

	if (!line)
		return (NULL);
	cursor = line;
	token_lst = NULL;
	new_token = NULL;
	while (*cursor)
	{
		debug("[tokenize_input] start tokenize - cursor : \'%s\'", cursor);
		cursor = tokenize(cursor, &token_lst, &new_token);
		if (!cursor)
			return (NULL);
	}
	debug("[tokenize_input] tokenize completed");
	new_token = create_token(NULL);
	new_token->type = TK_EOF;
	new_token->value = NULL;
	append_token_to_lst(&token_lst, new_token);
	debug("[tokenize_input] eof token appended");
	cur = token_lst;
	while (cur)
	{
		if (!(cur->value))
			break ;
		debug("[tokenize_input] token : %s", (char *)(cur->value));
		cur = cur->next;
	}
	return (token_lst);
}

static char	*tokenize(char *cursor, t_token **token_lst, t_token **new_token)
{
	t_token	*last_token;

	*new_token = create_token(cursor);
	if (!*new_token)
		return (cursor + 1);
	cursor += (ft_strlen((char *)((*new_token)->value)));
	debug("[tokenize] new_token created - value : %s", (char *)((*new_token)->value));
	if (check_ambigous(token_lst, *new_token))
	{
		clear_token_lst(token_lst);
		return (NULL);
	}
	last_token = *token_lst;
	while (last_token && last_token->next)
		last_token = last_token->next;
	if (last_token && last_token->type == TK_REDIR_HEREDOC)
	{
		debug("[tokenize] expand_heredoc_target start");
		expand_heredoc_target(new_token);
	}
	else if ((*new_token)->type == TK_WORD)
	{
		last_token = *new_token;
		*new_token = expand_token_refactor(last_token);
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

	last_token = *token_lst;
	while (last_token && last_token->next)
		last_token = last_token->next;
	if (!token || !last_token || !is_io_token(last_token)
		|| (token->type != TK_WORD))
		return (FALSE);
	expanded = expand_token_refactor(token);
	if (!expanded)
		return (FALSE);
	if (!expanded->next || !(expanded->next->type == TK_WORD))
		return (FALSE);
	ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putendl_fd(": ambigous redirect", STDERR_FILENO);
	clear_token_lst(&expanded);
	return (TRUE);
}

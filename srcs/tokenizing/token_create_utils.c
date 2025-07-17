/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:40:27 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/16 20:02:46 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "tokenizer.h"

static t_token_type	get_single_char_token_type(char *str);

t_token_type	get_token_type(char *str)
{
	if (!str || !*str || *str == ' ' || *str == '\t')
		return (TK_EMPTY);
	if ( *str == '\'')
		return (TK_WORD_WITH_SQUOTE);
	if ( *str == '\"')
		return (TK_WORD_WITH_DQUOTE);
	if ( *str == ';' || *str == '|' || *str == '&'
		|| *str == '<' || *str == '>'
		|| *str == '(' || *str == ')')
		return (get_single_char_token_type(str));
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
	return (TK_WORD);
}

void	set_token_value_from_str(t_token *target, char *str)
{
	if (target->type == TK_WORD_WITH_SQUOTE)
		target->value = extract_squote_word(str);
	else if (target->type == TK_WORD_WITH_DQUOTE)
		target->value = extract_dquote_word(str);
	else
		target->value = extract_normal_word(str);
}

void	set_token_value_from_type(t_token *target)
{
	if (target->type == TK_SEMICOLON)
		target->value = ft_strdup(STR_SEMICOLON);
	if (target->type == TK_PIPE)
		target->value = ft_strdup(STR_PIPE);
	if (target->type == TK_AMPERSAND)
		target->value = ft_strdup(STR_AMPERSAND);
	if (target->type == TK_REDIR_IN)
		target->value = ft_strdup(STR_REDIR_IN);
	if (target->type == TK_REDIR_OUT)
		target->value = ft_strdup(STR_REDIR_OUT);
	if (target->type == TK_REDIR_HEREDOC)
		target->value = ft_strdup(STR_REDIR_HEREDOC);
	if (target->type == TK_REDIR_APPEND)
		target->value = ft_strdup(STR_REDIR_APPEND);
	if (target->type == TK_LPAREN)
		target->value = ft_strdup(STR_LPAREN);
	if (target->type == TK_RPAREN)
		target->value = ft_strdup(STR_RPAREN);
	if (target->type == TK_AND_IF)
		target->value = ft_strdup(STR_AND_IF);
	if (target->type == TK_OR_IF)
		target->value = ft_strdup(STR_OR_IF);
	if (target->type == TK_EMPTY)
		target->value = NULL;
}

static t_token_type	get_single_char_token_type(char *str)
{
	if (*str == ';')
		return (TK_SEMICOLON);
	if (*str == '|')
		return (TK_PIPE);
	if (*str == '&')
		return (TK_AMPERSAND);
	if (*str == '<')
		return (TK_REDIR_IN);
	if (*str == '>')
		return (TK_REDIR_OUT);
	if (*str == '(')
		return (TK_LPAREN);
	else
		return (TK_RPAREN);
}

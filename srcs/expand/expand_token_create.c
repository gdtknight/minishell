/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_create.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:35:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 06:16:33 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "expand.h"

t_exp_token	*create_empty_exp_token(void)
{
	t_exp_token	*new_exp_token;

	new_exp_token = (t_exp_token *) malloc(sizeof(t_exp_token));
	if (!new_exp_token)
		return (NULL);
	new_exp_token->value = NULL;
	new_exp_token->qmask = NULL;
	new_exp_token->next = NULL;
	new_exp_token->prev = NULL;
	return (new_exp_token);
}

t_exp_token	*create_exp_token(char *value)
{
	t_exp_token	*new_exp_token;

	new_exp_token = create_empty_exp_token();
	new_exp_token->value = ft_strdup(value);
	new_exp_token->qmask = masking_token(value);
	return (new_exp_token);
}

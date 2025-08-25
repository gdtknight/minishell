/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_create.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:35:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 10:40:26 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "expand.h"

t_exp_token	*create_empty_exp_token(void)
{
	t_exp_token	*new_exp_token;

	new_exp_token = (t_exp_token *) ft_calloc(sizeof(t_exp_token), 1);
	if (!new_exp_token)
		return (NULL);
	new_exp_token->value = NULL;
	new_exp_token->qmask = NULL;
	return (new_exp_token);
}

t_exp_token	*create_exp_token(char *value)
{
	t_exp_token	*new_exp_token;

	if (!value)
		return (NULL);
	new_exp_token = create_empty_exp_token();
	new_exp_token->value = ft_strdup(value);
	new_exp_token->qmask = masking_token(value);
	return (new_exp_token);
}

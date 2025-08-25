/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_create.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:35:28 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 21:37:08 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file expand_token_create.c
 * @brief Functions to create and initialize t_exp_token structures
 *
 * This file contains helper functions to allocate and initialize
 * t_exp_token instances, including setting the value and quote mask.
 */

#include "libft.h"
#include "expand.h"

/**
 * @brief Create an empty expansion token.
 *
 * Allocates memory for a t_exp_token and initializes its members
 * to NULL.
 *
 * @return t_exp_token* Newly allocated empty token, or NULL on failure
 */
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

/**
 * @brief Create a t_exp_token from a string value.
 *
 * Allocates a new t_exp_token, duplicates the input string into
 * the value field, and generates a quote mask for the token.
 *
 * @param value Input string to initialize the token value
 * @return t_exp_token* Newly allocated token, or NULL if value is NULL
 */
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

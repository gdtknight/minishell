/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   masking_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:24:32 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 06:17:06 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "def.h"
#include "utils.h"
#include "expand.h"

static void	check_char(t_boolean *state, char *masking, char c);

char	*masking_token(const char *value)
{
	t_boolean	state[3];
	char		*masking;
	size_t		i;

	if (!value)
		return (NULL);
	masking = malloc(sizeof(char) * (ft_strlen(value) + 1));
	if (!masking)
		return (NULL);
	state[IN_SQUOTE] = FALSE;
	state[IN_DQUOTE] = FALSE;
	state[IN_ESCAPE] = FALSE;
	i = 0;
	while (value[i])
	{
		check_char(state, &(masking[i]), value[i]);
		i++;
	}
	masking[i] = '\0';
	return (masking);
}

static void	check_char(t_boolean *state, char *masking, char c)
{
	*masking = MASK_UNQUOTED;
	if (state[IN_ESCAPE])
		state[IN_ESCAPE] = FALSE;
	else if (c == '\\' && !state[IN_SQUOTE])
		state[IN_ESCAPE] = TRUE;
	else if (c == '\'' && !state[IN_DQUOTE])
	{
		state[IN_SQUOTE] = !state[IN_SQUOTE];
		*masking = MASK_QUOTED;
	}
	else if (c == '"' && !state[IN_SQUOTE])
	{
		state[IN_DQUOTE] = !state[IN_DQUOTE];
		*masking = MASK_QUOTED;
	}
	else if (is_ifs(c) && (!state[IN_SQUOTE] && !state[IN_DQUOTE]))
		*masking = MASK_IFS;
}

char	*expand_mask(char *quote_mask, size_t pos, char *before, char *after)
{
	char	*new_mask;
	size_t	len_before;
	size_t	len_after;
	size_t	len_old;
	size_t	len_new;

	len_before = ft_strlen(before);
	len_after = ft_strlen(after);
	len_old = ft_strlen(quote_mask);
	len_new = len_old - len_before + len_after;
	new_mask = ft_calloc(sizeof(char), (len_new + 1));
	if (!new_mask)
		return (NULL);
	ft_memcpy(new_mask, quote_mask, pos);
	ft_memset(new_mask + pos, '0', len_after);
	ft_memcpy(new_mask + pos + len_after,
		quote_mask + pos + len_before,
		len_old - (pos + len_before));
	new_mask[len_new] = '\0';
	return (new_mask);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 01:33:49 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/25 15:15:36 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "def.h"
#include "utils.h"

t_boolean	is_valid_pair(char *input)
{
	t_boolean	state[3];
	char		c;

	state[IN_ESCAPE] = FALSE;
	state[IN_SQUOTE] = FALSE;
	state[IN_DQUOTE] = FALSE;
	while (*input)
	{
		c = *input++;
		if (state[IN_ESCAPE])
			state[IN_ESCAPE] = FALSE;
		else if (c == '\\' && !state[IN_SQUOTE])
			state[IN_ESCAPE] = TRUE;
		else if (c == '\'' && !state[IN_DQUOTE])
			state[IN_SQUOTE] = !state[IN_SQUOTE];
		else if (c == '"' && !state[IN_SQUOTE])
			state[IN_DQUOTE] = !state[IN_DQUOTE];
	}
	return (!state[IN_ESCAPE] && !state[IN_SQUOTE] && !state[IN_DQUOTE]);
}

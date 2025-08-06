/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:44:13 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/02 21:26:53 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashmap.h"
#include "tokenizer.h"
#include "shell_data.h"

t_status	builtin_exit(t_token **tk_lst, t_hash_map *map)
{
	t_token	*cur;

	clear_hashmap(map);
	while (*tk_lst)
	{
		cur = *tk_lst;
		*tk_lst = (*tk_lst)->next;
		free (cur->value);
		cur->next = NULL;
		free(cur);
	}
	exit ((get_shell_data()->last_status));
}

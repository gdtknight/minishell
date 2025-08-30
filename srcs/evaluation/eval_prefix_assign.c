/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_prefix_assign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 08:29:26 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/30 08:47:03 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "ast.h"

static void		append_assign(char ***envp, t_syntax_node *assign_node);
static size_t	get_size(char **envp);

void	append_prefix_assign(char ***envp, t_syntax_node *node)
{
	t_syntax_node	*cur;

	if (!node || node->eval == OFF)
		return ;
	cur = node;
	while (cur && cur->type == NODE_CMD_PREFIX)
	{
		if (cur->value.b_node.left->type == NODE_ASSIGN_WORD)
			append_assign(envp, cur->value.b_node.left);
		cur = cur->value.b_node.right;
	}
	if (cur->type == NODE_ASSIGN_WORD)
		append_assign(envp, cur);
}

static void	append_assign(char ***envp, t_syntax_node *assign_node)
{
	int		idx;
	char	**new_envp;

	new_envp = (char **) ft_calloc(sizeof(char *), get_size(*envp) + 1);
	if (!new_envp)
		return ;
	idx = 0;
	while ((*envp)[idx])
	{
		new_envp[idx] = (*envp)[idx];
		idx++;
	}
	new_envp[idx++] = ft_strdup(assign_node->value.assign_word);
	new_envp[idx] = NULL;
	free(*envp);
	*envp = new_envp;
}

static size_t	get_size(char **envp)
{
	size_t	size;
	char	**cur;

	size = 0;
	if (!envp)
		return (size);
	cur = envp;
	while (*cur)
	{
		size++;
		cur++;
	}
	size++;
	return (size);
}

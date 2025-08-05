/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:46:41 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 20:54:39 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "def.h"

static void			fill_dp(
						t_boolean **dp,
						int idxes[2],
						const char *s,
						const char *p);
static t_boolean	**create_dp_arr(int n, int m);

t_boolean	wildcard_match(const char *s, const char *p)
{
	int			slen;
	int			plen;
	t_boolean	**dp;
	int			idxes[2];

	slen = ft_strlen(s);
	plen = ft_strlen(p);
	dp = create_dp_arr(slen, plen);
	dp[0][0] = TRUE;
	idxes[1] = 0;
	while (++idxes[1] <= plen)
		if (p[idxes[1] - 1] == '*')
			dp[0][idxes[1]] = dp[0][idxes[1] - 1];
	idxes[0] = 0;
	while (++idxes[0] <= slen)
	{
		idxes[1] = 0;
		while (++idxes[1] <= plen)
			fill_dp(dp, idxes, s, p);
	}
	return (dp[slen][plen]);
}

static void	fill_dp(
				t_boolean **dp,
				int idxes[2],
				const char *s,
				const char *p)
{
	int	s_idx;
	int	p_idx;

	s_idx = idxes[0];
	p_idx = idxes[1];
	if (p[p_idx - 1] == '*')
		dp[s_idx][p_idx] = dp[s_idx][p_idx - 1] || dp[s_idx - 1][p_idx];
	else if (p[p_idx - 1] == '?' || p[p_idx - 1] == s[s_idx - 1])
		dp[s_idx][p_idx] = dp[s_idx -1][p_idx -1];
	else
		dp[s_idx][p_idx] = FALSE;
}

static t_boolean	**create_dp_arr(int n, int m)
{
	t_boolean	**dp;
	int			r;
	int			c;

	dp = (t_boolean **) malloc(sizeof(t_boolean *) * (n + 1));
	r = -1;
	while (++r <= n)
	{
		dp[r] = (t_boolean *) malloc(sizeof(t_boolean) * (m + 1));
		c = -1;
		while (++c <= m)
			dp[r][c] = FALSE;
	}
	return (dp);
}

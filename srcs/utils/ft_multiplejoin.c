/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multiplejoin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:19:46 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/01 19:20:04 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "utils.h"

char	*ft_multiplejoin(const char *s1, const char *s2, const char *s3)
{
	char	*new_str;
	char	*temp;

	new_str = ft_strjoin(s1, s2);
	temp = new_str;
	new_str = ft_strjoin(temp, s3);
	free(temp);
	return (new_str);
}

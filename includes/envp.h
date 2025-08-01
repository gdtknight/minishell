/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:47:24 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/31 16:52:52 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

/* --- envp_utils.c --- */

char 	**lst_from_hashmap(t_hash_map *map);
char	*extract_key(char *envp);
char	*extract_value(char *envp);

#endif

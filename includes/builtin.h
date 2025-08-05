/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:19:29 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/02 21:32:42 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "hashmap.h"
# include "tokenizer.h"

int		builtin_cd(char *path, t_hash_map *map);
int		builtin_export(char *line, t_hash_map *map);
int		builtin_pwd(void);
int		builtin_unset(char *line, t_hash_map *map);
void	builtin_echo(char *line, int flag);
void	builtin_env(t_hash_map map);
int		builtin_exit(t_token **tk_lst, t_hash_map *map);

#endif

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

# include "def.h"
# include "hashmap.h"
# include "tokenizer.h"

t_status	builtin_cd(char *path, t_hash_map *map);
t_status	builtin_export(char *line, t_hash_map *map);
t_status	builtin_pwd(void);
t_status	builtin_unset(char *line, t_hash_map *map);
t_status	builtin_echo(char *line, t_toggle flag);
t_status	builtin_env(t_hash_map map);
t_status	builtin_exit(t_token **tk_lst, t_hash_map *map);

#endif

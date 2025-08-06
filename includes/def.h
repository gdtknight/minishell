/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo < jyoo@student.42gyeongsan.kr >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:52:20 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/06 18:25:23 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_H
# define DEF_H

typedef enum e_result
{
	COMPLETED = 1,
	INCOMPLETED = 0
}	t_result;

typedef enum e_status
{
	SUCCESS = 1,
	FAIL = 0
}	t_status;

typedef enum e_builtin_status
{
	BUILTIN_SUCCESS = 0,
	BUILTIN_FAIL = 1
}	t_builtin_status;

typedef enum e_boolean
{
	TRUE = 1,
	FALSE = 0
}	t_boolean;

typedef enum e_flag
{
	ON = 1,
	OFF = 0
}	t_flag;

#endif

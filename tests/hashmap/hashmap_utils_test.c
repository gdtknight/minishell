/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:27:47 by yoshin            #+#    #+#             */
/*   Updated: 2025/07/30 01:27:47 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft.h"

#include "hashmap.h"

void	create_new_entry_success_test(void)
{
	static char		*key = "test_key";
	static char		*value = "test_value";

	t_hash_entry	*test_entry;

	test_entry = create_new_entry(key, value);
	if (ft_strlen(key) != ft_strlen(test_entry->key)
		|| ft_strncmp(key, test_entry->key, ft_strlen(key)) != 0)
		printf("Entry key(%s) is not equal to origin(%s)\n", test_entry->key, key);
	if (ft_strlen(value) != ft_strlen(test_entry->value)
		|| ft_strncmp(value, test_entry->value, ft_strlen(value)) != 0)
		printf("Entry value(%s) is not equal to origin(%s)\n", test_entry->value, value);
	printf("[SUCCESS] create_new_entry_test\n");
	free(test_entry->key);
	free(test_entry->value);
	free(test_entry);
}

void	equal_key_success_test(void)
{
	static char	*ori_key = "test_key";
	static char *target_key = "test_key";

	if (equal_key(ori_key, target_key))
		printf("[SUCCESS] %s is equal to %s\n", ori_key, target_key);
	else
		printf("[FAIL] %s is not equal to %s\n", ori_key, target_key);
}

void	equal_key_fail_test(void)
{
	static char	*ori_key = "ori_key";
	static char *target_key = "target_key";

	if (equal_key(ori_key, target_key))
		printf("[FAIL] %s is equal to %s\n", ori_key, target_key);
	else
		printf("[SUCCESS] %s is not equal to %s\n", ori_key, target_key);
}

int	main(void)
{
	create_new_entry_success_test();
	equal_key_success_test();
	equal_key_fail_test();
}

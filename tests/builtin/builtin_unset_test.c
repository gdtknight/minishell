/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:30:00 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/05 16:21:04 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#include "def.h"
#include "shell_data.h"
#include "builtin.h"

static void assert_result(const char *desc, int actual_status, int expected_status)
{
    if (actual_status == expected_status)
        printf("✅ [%s] returned %d (expected %d)\n", desc, actual_status, expected_status);
    else
        printf("❌ [%s] returned %d (expected %d)\n", desc, actual_status, expected_status);
}

static void assert_env_absence(char *key)
{
    char *val = get_value(&get_shell_data()->envp_map, key);
    if (!val)
        printf("✅ %s correctly absent\n", key);
    else
        printf("❌ %s should not be present (got %s)\n", key, val);
}

/*
static void assert_env_presence(char *key, const char *expected)
{
    char *val = get_value(&get_shell_data()->envp_map, key);
    if (val && strcmp(val, expected) == 0)
        printf("✅ %s=%s present\n", key, expected);
    else
        printf("❌ %s not found or wrong value (got %s)\n", key, val ? val : "(null)");
}*/

int main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;

    printf("========== INIT ==========\n");
    init_shell_data(envp);

    // Setup: add variables
    builtin_export("VAR1=value1 VAR2=value2 VAR3=value3 _VAR4=value4", &get_shell_data()->envp_map);
    printf("Initial environment:\n");
    builtin_env(get_shell_data()->envp_map);

    printf("\n========== SINGLE KEY UNSET ==========\n");
    assert_result("unset VAR1", builtin_unset("VAR1", &get_shell_data()->envp_map), 0);
    assert_env_absence("VAR1");

    printf("\n========== MULTIPLE KEYS UNSET ==========\n");
    assert_result("unset VAR2 and VAR3", builtin_unset("VAR2 VAR3", &get_shell_data()->envp_map), 0);
    assert_env_absence("VAR2");
    assert_env_absence("VAR3");

    printf("\n========== NON-EXISTENT KEY ==========\n");
    assert_result("unset NOTFOUND", builtin_unset("NOTFOUND", &get_shell_data()->envp_map), 0);
    assert_env_absence("NOTFOUND");

    printf("\n========== UNDERSCORE KEY ==========\n");
    assert_result("unset _VAR4", builtin_unset("_VAR4", &get_shell_data()->envp_map), 0);
    assert_env_absence("_VAR4");

    printf("\n========== INVALID KEYS ==========\n");
    assert_result("unset 1BAD", builtin_unset("1BAD", &get_shell_data()->envp_map), 0);
    assert_result("unset BAD-KEY", builtin_unset("BAD-KEY", &get_shell_data()->envp_map), 0);

    printf("\n========== EMPTY INPUT ==========\n");
    assert_result("unset \"\"", builtin_unset("", &get_shell_data()->envp_map), 1);
    assert_result("unset NULL", builtin_unset(NULL, &get_shell_data()->envp_map), 1);

    printf("\n========== FINAL ENV MAP ==========\n");
    builtin_env(get_shell_data()->envp_map);

    printf("\n========== TEST COMPLETE ==========\n");
	clear_hashmap(&get_shell_data()->envp_map);

    return 0;
}

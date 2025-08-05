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

static void assert_env_value(char *key, const char *expected)
{
	char *val = get_value(&get_shell_data()->envp_map, key);
	if (val && strcmp(val, expected) == 0)
		printf("✅ %s=%s\n", key, expected);
	else
		printf("❌ %s not set correctly (got %s)\n", key, val ? val : "(null)");
}

static void assert_env_absence(char *key)
{
	char *val = get_value(&get_shell_data()->envp_map, key);
	if (!val)
		printf("✅ %s correctly absent\n", key);
	else
		printf("❌ %s should not be present (got %s)\n", key, val);
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;

	printf("========== INIT ==========\n");
	init_shell_data(envp);

	printf("\n========== BASIC ADD ==========\n");
	assert_result("VAR1=value1", builtin_export("VAR1=value1", &get_shell_data()->envp_map), 1);
	assert_env_value("VAR1", "value1");

	printf("\n========== MULTIPLE KEYS ==========\n");
	assert_result("VAR2 VAR3", builtin_export("VAR2=value2 VAR3=value3", &get_shell_data()->envp_map), 1);
	assert_env_value("VAR2", "value2");
	assert_env_value("VAR3", "value3");

	printf("\n========== EMPTY VALUE ==========\n");
	assert_result("VAR4=", builtin_export("VAR4=", &get_shell_data()->envp_map), 1);
	assert_env_value("VAR4", "");

	printf("\n========== UNDERSCORE VAR ==========\n");
	assert_result("_VAR5=underscore", builtin_export("_VAR5=underscore", &get_shell_data()->envp_map), 1);
	assert_env_value("_VAR5", "underscore");

	printf("\n========== NUMERIC START (INVALID) ==========\n");
	assert_result("1VAR=wrong", builtin_export("1VAR=wrong", &get_shell_data()->envp_map), 0);
	assert_env_absence("1VAR");

	printf("\n========== SPECIAL CHARACTER IN KEY ==========\n");
	assert_result("VA-R=wrong", builtin_export("VA-R=wrong", &get_shell_data()->envp_map), 0);
	assert_result("VAR$BAD=wrong", builtin_export("VAR$BAD=wrong", &get_shell_data()->envp_map), 0);
	assert_env_absence("VA-R");
	assert_env_absence("VAR$BAD");

	printf("\n========== MISSING KEY ==========\n");
	assert_result("=novar", builtin_export("=novar", &get_shell_data()->envp_map), 0);

	printf("\n========== MULTIPLE ERROR INPUTS ==========\n");
	assert_result("VAR6 + 1BAD + BAD-2", builtin_export("VAR6=ok 1BAD=fail BAD-2=fail2", &get_shell_data()->envp_map), 0);
	assert_env_value("VAR6", "ok");
	assert_env_absence("1BAD");
	assert_env_absence("BAD-2");

	printf("\n========== UPDATE EXISTING KEY ==========\n");
	assert_result("VAR1=newvalue", builtin_export("VAR1=newvalue", &get_shell_data()->envp_map), 1);
	assert_env_value("VAR1", "newvalue");

	printf("\n========== ONLY KEY (NO VALUE) ==========\n");
	assert_result("ONLYKEY", builtin_export("ONLYKEY", &get_shell_data()->envp_map), 1);
	assert_env_value("ONLYKEY", "");

	printf("\n========== EMPTY INPUT ==========\n");
	assert_result("Empty line", builtin_export("", &get_shell_data()->envp_map), 1);

	printf("\n========== LONG VALUE ==========\n");
	assert_result("LONGVAR=aaa...", builtin_export("LONGVAR=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &get_shell_data()->envp_map), 1);
	assert_env_value("LONGVAR", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	printf("\n========== FINAL HASHMAP DUMP ==========\n");
	builtin_env(get_shell_data()->envp_map);

	printf("\n========== TEST COMPLETE ==========\n");
	clear_hashmap(&get_shell_data()->envp_map);
	return 0;
}

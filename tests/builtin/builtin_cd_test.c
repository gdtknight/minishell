#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "def.h"
#include "shell_data.h"
#include "builtin.h"

static void print_pwd_vars(t_hash_map *map)
{
    char *pwd = get_value(map, "PWD");
    char *oldpwd = get_value(map, "OLDPWD");
    printf("PWD=%s | OLDPWD=%s\n", pwd ? pwd : "(null)", oldpwd ? oldpwd : "(null)");
}

/*
tmp 폴더에 해당 디렉토리가 존재해야함.
다음의 명령어를 입력하시오.
mkdir -p /tmp/cd_test/normal_dir
mkdir -p /tmp/cd_test/denied_dir
chmod 000 /tmp/cd_test/denied_dir
mkdir -p /tmp/cd_test/old_dir
ln -s /tmp/cd_test/normal_dir /tmp/cd_test/link_dir
*/

int main(int argc, char *argv[], char *envp[])
{
    (void) argc;
    (void) argv;

    printf("========== INIT ==========\n");
    init_shell_data(envp);

    printf("\n========== CD DASH DASH (HOME) ==========\n");
    builtin_cd("--", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== NORMAL DIRECTORY ==========\n");
    builtin_cd("/tmp/cd_test/normal_dir", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== TOO MANY ARGS ==========\n");
    builtin_cd("arg1 arg2", &get_shell_data()->envp_map);

    printf("\n========== NONEXISTENT DIRECTORY ==========\n");
    builtin_cd("/tmp/cd_test/missing_dir", &get_shell_data()->envp_map);

    printf("\n========== PERMISSION DENIED ==========\n");
    builtin_cd("/tmp/cd_test/denied_dir", &get_shell_data()->envp_map);

    printf("\n========== SYMBOLIC LINK ==========\n");
    builtin_cd("/tmp/cd_test/link_dir", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== OLD DIRECTORY ==========\n");
    builtin_cd("/tmp/cd_test/old_dir", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== CD DASH (Previous Dir) ==========\n");
    builtin_cd("-", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);


	printf("\n========== CD '' or \"\"==========\n");
    builtin_cd("", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== NO HOME SET ==========\n");
    builtin_unset("HOME", &get_shell_data()->envp_map);
    builtin_cd(NULL, &get_shell_data()->envp_map);

    printf("\n========== CURRENT DIR (.) ==========\n");
    builtin_cd(".", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

	printf("\n========== CURRENT DIR (..) ==========\n");
    builtin_cd("..", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== COMPLEX RELATIVE PATH ==========\n");
    builtin_cd("./../cd_test/normal_dir", &get_shell_data()->envp_map);
    print_pwd_vars(&get_shell_data()->envp_map);

    printf("\n========== TEST COMPLETE ==========\n");
	clear_hashmap(&get_shell_data()->envp_map);

    return 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 01:15:23 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/05 16:21:02 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"
#include "def.h"
#include "shell_data.h"
#include "builtin.h"

int main(int argc, char *argv[], char *envp[])
{
    (void) argc;
    (void) argv;

    // Shell 데이터 초기화
    init_shell_data(envp);

    printf("========== BASIC PWD TEST ==========\n");
    builtin_pwd();

    printf("\n========== CHANGE DIRECTORY TEST ==========\n");
    char *orig_dir = getcwd(NULL, 0);
    if (chdir("/") == 0)
    {
        printf("Changed to root directory:\n");
        builtin_pwd();
    }
    else
    {
        perror("chdir to / failed");
    }

    printf("\n========== RESTORE DIRECTORY ==========\n");
    if (orig_dir && chdir(orig_dir) == 0)
    {
        builtin_pwd();
    }
    else
    {
        perror("restore directory failed");
    }

    printf("\n========== INVALID DIRECTORY TEST ==========\n");
    if (chdir("/nonexistent_dir") != 0)
    {
        perror("Expected failure changing directory");
    }
    builtin_pwd();

    printf("\n========== MULTIPLE CALLS ==========\n");
    builtin_pwd();
    builtin_pwd();

	clear_hashmap(&get_shell_data()->envp_map);

    free(orig_dir);

    return 0;
}
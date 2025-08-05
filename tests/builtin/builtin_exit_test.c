/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 16:30:00 by jyoo              #+#    #+#             */
/*   Updated: 2025/08/05 16:21:07 by jyoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "hashmap.h"
#include "def.h"
#include "shell_data.h"
#include "builtin.h"
#include "tokenizer.h"

static t_token *create_tokens(void)
{
    t_token *head = malloc(sizeof(t_token));
    t_token *arg = malloc(sizeof(t_token));

    head->value = strdup("exit");
    head->next = arg;

    arg->value = strdup("dummy_arg");
    arg->next = NULL;

    return head;
}

static void run_exit_test(const char *desc, int last_status, int expected_exit)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("[Child] %s (last_status=%d)\n", desc, last_status);
        get_shell_data()->last_status = last_status;

        t_token *tokens = create_tokens();
        put_key_value(&get_shell_data()->envp_map, "TEST_KEY", "TEST_VALUE");

        builtin_exit(&tokens, &get_shell_data()->envp_map);

        printf("[Child] ERROR: builtin_exit did not exit.\n");
        exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int code = WEXITSTATUS(status);
            printf("[Parent] %s → exited with code: %d", desc, code);
            if (code == expected_exit)
                printf(" ✅ (PASS)\n\n");
            else
                printf(" ❌ (FAIL: expected %d)\n\n", expected_exit);
        }
        else
        {
            printf("[Parent] %s → ❌ did not exit normally.\n\n", desc);
        }
    }
}

int main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;

    printf("========== INIT ==========\n");
    init_shell_data(envp);

    run_exit_test("Default exit (0)", 0, 0);
    run_exit_test("Custom last_status (42)", 42, 42);
    run_exit_test("Error code (1)", 1, 1);
    run_exit_test("Max byte code (255)", 255, 255);
    run_exit_test("Wrapped code (>255)", 300, (300 % 256));

    printf("========== TEST COMPLETE ==========\n");
	clear_hashmap(&get_shell_data()->envp_map);

    return 0;
}

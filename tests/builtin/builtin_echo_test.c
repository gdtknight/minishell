#include <stdio.h>
#include <string.h>
#include "def.h"
#include "builtin.h"

int main(void)
{
    printf("========== BASIC ==========\n");
    builtin_echo("Hello", OFF);

    printf("\n========== NULL INPUT ==========\n");
    builtin_echo(NULL, OFF);

	printf("\n========== '' or \"\" INPUT ==========\n");
    builtin_echo("", OFF);

    printf("\n========== LONG STRING ==========\n");
    char long_str[5000];
    memset(long_str, 'A', sizeof(long_str) - 1);
    long_str[sizeof(long_str) - 1] = '\0';
    builtin_echo(long_str, OFF);

    printf("\n========== ANSI COLOR ==========\n");
    builtin_echo("\033[31mRED\033[0m", OFF);

    printf("\n========== MULTIPLE SPACES AND TABS ==========\n");
    builtin_echo("word1     word2\tword3", OFF);

    printf("\n========== UNICODE ==========\n");
    builtin_echo("안녕하세요 😊", OFF);

    printf("\n========== ESCAPED CHARACTERS ==========\n");
    builtin_echo("Hello\\nWorld", OFF);

    printf("\n========== MULTIPLE -n FLAGS SIMULATION ==========\n");
    builtin_echo("text", ON);
    builtin_echo("text again", ON);

    printf("\n========== STRING WITH NEWLINE ==========\n");
    builtin_echo("text\n", OFF);

    printf("\n========== ONLY SPACES ==========\n");
    builtin_echo("     ", OFF);

    printf("\n========== MULTIPLE CALLS ==========\n");
    for (int i = 0; i < 10; i++) {
        builtin_echo("Repeated call", OFF);
    }

    printf("\n========== TEST COMPLETE ==========\n");
    return 0;
}

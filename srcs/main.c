/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyoo <jyoo@student.42gyeongsan.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 10:46:09 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/29 15:36:16 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @brief Entry point and main loop of the minishell program.
 *
 * This file contains the main function of the minishell, the interactive loop,
 * and the core input processing functions. It initializes shell data, manages
 * signals, and handles user input using GNU Readline.
 */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "libft.h"
#include "get_next_line_bonus.h"
#include "shell.h"
#include "tokenizer.h"
#include "utils.h"
#include "eval.h"

static void	interactive_mode(void);
static void	read_shell_input(void);
static void	process_input(char *input);

/**
 * @brief Main entry point of the minishell.
 *
 * Initializes signals and shell data, then either runs in interactive mode
 * (if no arguments are passed) or exits immediately. Before termination,
 * it restores signal settings and clears allocated shell resources.
 *
 * @param argc Number of command-line arguments.
 * @param argv List of command-line arguments.
 * @param envp Environment variables inherited from the parent process.
 * @return Exit status of the minishell.
 */
int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	init_minishell_signal();
	if (!init_shell_data(envp))
		exit(errno);
	if (argc == 1)
		interactive_mode();
	restore_signal();
	clear_shell_data();
	exit(get_shell_data()->last_status);
}

/**
 * @brief Runs the interactive loop of the minishell.
 *
 * Continuously prompts the user for input using Readline, tokenizes and
 * processes the input, and executes commands until the shell is terminated
 * or the user triggers an exit (e.g., Ctrl+D or the `exit` builtin).
 */
static void	interactive_mode(void)
{
	while (!(get_shell_data()->is_exit))
	{
		read_shell_input();
		if ((get_shell_input())->input_line == NULL)
			break ;
		if (ft_strncmp((get_shell_input())->input_line, "", \
				ft_strlen("") + 1) == 0)
			continue ;
		if (ft_strncmp((get_shell_input())->input_line, "\n", \
				ft_strlen("\n") + 1) == 0)
		{
			ft_putendl_fd("\n", STDOUT_FILENO);
			clear_shell_input();
			continue ;
		}
		process_input((get_shell_input())->input_line);
	}
}

/**
 * @brief Tokenizes, parses, and evaluates a user input line.
 *
 * Validates input syntax (paired characters and sequence rules),
 * converts the input into tokens, builds an AST, evaluates heredocs,
 * and finally executes commands.
 *
 * @param input The raw user input string to process.
 */
static void	process_input(char *input)
{
	t_token	*input_token;

	add_history((get_shell_input())->input_line);
	if (!is_valid_pair(input))
	{
		ft_putstr_fd("Invalid input\n", STDERR_FILENO);
		(get_shell_data())->last_status = EXIT_FAILURE;
		return ;
	}
	(get_shell_input())->input_token = tokenize_input(input);
	if (!is_valid_sequence((get_shell_input())->input_token))
	{
		ft_putstr_fd("Invalid input\n", STDERR_FILENO);
		(get_shell_data())->last_status = EXIT_FAILURE;
		return ;
	}
	input_token = (get_shell_input())->input_token;
	(get_shell_input())->input_node = parse_input(&input_token);
	eval_heredoc((get_shell_input())->input_node);
	eval((get_shell_input())->input_node);
	clear_heredoc_input();
	clear_shell_input();
}

static void	read_shell_input(void)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		rl_replace_line("", 0);
		(get_shell_input())->input_line = readline(PROMPT);
	}
	else if (isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))
	{
		ft_putstr_fd(PROMPT, STDERR_FILENO);
		(get_shell_input())->input_line = get_next_line(STDIN_FILENO);
	}
	else
	{
		(get_shell_input())->input_line = get_next_line(STDIN_FILENO);
	}
}

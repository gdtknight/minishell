_This project has been created as part of the 42 curriculum by yoshin, jyoo._

# minishell

A minimalist Bash-like interactive shell written in C, built with only the system calls and library functions allowed by the 42 minishell subject (v10.0).

## Description

`minishell` reproduces a small but faithful subset of Bash's behavior: an interactive prompt with line editing and history, command parsing with quote handling, parameter expansion, redirections, pipelines, signal handling, and seven builtin commands. The project is an exercise in the Unix process model — every external command goes through `fork(2)` and `execve(2)`, with file descriptors arranged for redirections and pipes via `dup2(2)`.

The implementation walks raw input through a fixed pipeline:

```
read → tokenize → expand → parse (AST) → collect heredocs → eval → execute
```

State is held in two function-local `static` singletons accessed via getter functions, satisfying the subject's "no global variables except for one signal number" rule.

## Features

- Interactive prompt with line editing and history (GNU Readline)
- Quoting: `'…'` (literal), `"…"` (preserves all metacharacters except `$`)
- Parameter expansion: `$VAR`, `$?`
- Tilde expansion (`~`)
- IFS-based field splitting on unquoted expansion results
- Redirections: `<`, `>`, `>>`, `<<` (heredoc, with delimiter-quoted body literalization)
- Pipelines: `|` (n-ary, evaluated as a recursive binary tree)
- Seven builtins: `cd`, `echo` (with `-n`), `env`, `exit`, `export`, `pwd`, `unset`
- Signal handling matching Bash interactive behavior:
  - `Ctrl-C` (SIGINT): redraws a fresh prompt
  - `Ctrl-D` (EOF): exits the shell at an empty prompt
  - `Ctrl-\` (SIGQUIT): ignored at the prompt; default behavior in children
- PATH search with the standard `126` / `127` exit-code conventions
- Per-command environment variable assignment (`VAR=value cmd`)

## Instructions

### Requirements

- A C compiler (`cc`)
- GNU Readline development headers
  - Linux (Debian/Ubuntu): `sudo apt install libreadline-dev`
  - macOS: `brew install readline` (looked up at `/opt/homebrew/opt/readline` or `/usr/local/opt/readline`)
- GNU Make

### Build

```sh
make              # build ./minishell
make debug=1 all  # debug build (-g -DDEBUG, propagates to libft)
make re           # full rebuild (fclean + all)
make clean        # remove build artifacts
make fclean       # remove build artifacts and the binary
```

The bundled `libft/` is built automatically on first invocation.

### Run

```sh
./minishell                       # interactive mode
./minishell < script.sh           # non-interactive: stdin from file
echo "ls -la" | ./minishell       # non-interactive: stdin from pipe
```

### Examples

```sh
$ ./minishell
minishell> echo "hello, $USER"
hello, yoshin

minishell> ls -la | grep '^d' | wc -l
3

minishell> cat << EOF > out.txt
> first line for $USER
> second line
> EOF
minishell> cat out.txt
first line for yoshin
second line

minishell> export GREETING="hi" && env | grep GREETING
GREETING=hi
```

## Project Structure

```
includes/        Public headers (def, shell, ast, tokenizer, expand, ...)
srcs/
  main.c         Entry point and main REPL loop
  tokenize/      Lexer: raw input → token list
  expand/        Variable, tilde, IFS expansion (parallel quote-mask)
  ast/           Recursive-descent parser → t_syntax_node tree
  evaluation/    AST walker: pipelines, commands, redirections, heredoc
  execution/     execve, PATH search, builtin dispatch
  builtin/       cd, echo, env, exit, export, pwd, unset
  hashmap/       1024-bucket env-var store
  shell/         Singletons, signals, tty restore
  utils/         get_next_line, char predicates, env utils
libft/           42's standard library replacement (vendored)
wiki/            Study notes mirrored to the GitHub Wiki
report/          Internal refactoring audit
minish.md        Subject specification (v10.0)
```

For implementation details and architectural rationale, see [`CLAUDE.md`](CLAUDE.md).

## Resources

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — primary reference for any unspecified behavior
- [POSIX Shell Command Language (XCU §2)](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [GNU Readline Library Manual](https://tiswww.case.edu/php/chet/readline/readline.html)
- *Advanced Programming in the UNIX Environment*, W. Richard Stevens & Stephen A. Rago — process model, file descriptors, signals
- Stephen Brennan, [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/) — minimal shell tutorial that inspired the initial scaffolding
- Project's [GitHub Wiki](https://github.com/gdtknight/minishell/wiki) — study notes used while building the shell

### AI usage

Source code under `srcs/` and `includes/` was authored by hand by the project members. AI assistance (Claude Code) was used for the following non-source-code tasks:

- Drafting and maintaining the contributor-facing documentation (`CLAUDE.md`, `wiki/`, `report/`)
- Auditing the codebase against the subject's allow-listed function list
- Comparing the implementation against documented best practices and producing a prioritized refactoring plan

All AI-generated documentation was reviewed for accuracy by the authors before being committed.

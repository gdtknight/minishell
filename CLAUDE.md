# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

42 minishell — a bash-like interactive shell. Depends on GNU Readline and the bundled `libft/` (built automatically on first `make`). Targets Linux and macOS; readline include/lib paths are selected by `uname` in the top-level Makefile.

## Build / Run

```sh
make              # build ./minishell (also copied from ./bin/minishell)
make debug=1 all  # rebuild with -g -DDEBUG; propagates to libft as debug=1
make re           # fclean + all
make clean        # remove build/ and tests/build
make fclean       # clean + remove ./minishell, ./bin, libft archive
./minishell       # interactive mode
./minishell -c "echo hi | cat"   # single-command mode
```

Notes:
- `SRCS` in the Makefile is listed explicitly (Norm: no wildcards). When adding a `.c`, append it to `SRCS` in the matching module group.
- `bonus` rule is empty — bonus features are compiled into the main binary, not gated.
- A `test` / `do_test` scaffold exists but no `tests/` directory is checked in; treat the test rules as dormant.
- macOS readline is expected at `/opt/homebrew/opt/readline` or `/usr/local/opt/readline`.

## Pipeline (big picture)

`srcs/main.c` drives a loop: `read_shell_input → is_valid_pair → tokenize_input → is_valid_sequence → parse_input → eval_heredoc → eval`. Each stage lives in its own module and communicates via two process-wide singletons accessed through getter functions (never as globals).

```
tokenize/   raw string → t_token doubly-linked list (quote/mask-aware)
expand/     parameter ($VAR, $?), tilde, heredoc expansion + IFS field split
            uses a parallel "quote mask" string (MASK_QUOTED/UNQUOTED/IFS)
ast/        recursive-descent parser → t_syntax_node tree
            grammar roots: list → and_or → pipeline → command
            node kinds in t_node_type (simple/compound, prefix/suffix, io_redir…)
evaluation/ tree walker: eval, eval_pipeline, eval_command, eval_io_redir,
            eval_heredoc (runs before eval to pre-capture heredoc bodies),
            eval_prefix_assign (VAR=val cmd)
execution/  execve path resolution + builtin dispatch table
builtin/    cd, echo, env, exit, export, pwd, unset (one file per builtin,
            _utils split when a file would exceed Norm's 5-function/file limit)
hashmap/    1024-bucket chaining map used as the env store
            (t_shell_data.envp_map); char **envp materialized on demand
            via lst_from_hashmap
shell/      process-wide state + signal handling + tty restore
utils/      gnl, ft_lltoa, pair validator, env key/value extractors, char preds
```

### Singletons

- `get_shell_data()` (`srcs/shell/shell_data.c`) — env hashmap, `last_status`, child/redir flags, `is_exit`. Initialized once in `main` via `init_shell_data(envp)`.
- `get_shell_input()` (`srcs/shell/shell_input.c`) — per-line scratch: raw line, token list, AST root. Cleared with `clear_shell_input()` each iteration.

New code should reach state through these getters rather than plumbing pointers.

### Status / types

All modules use the enums in `includes/def.h` (`t_result`, `t_status`, `t_boolean`, `t_toggle`) instead of raw `int`/`bool`. Return `SUCCESS`/`FAILURE`/`ERROR` from evaluation-layer functions; `COMPLETED`/`INCOMPLETED` from lifecycle ones (init, list ops).

### Expansion quote-mask

`expand/masking_utils.c` produces a parallel mask string over each token value marking each byte as `MASK_QUOTED`, `MASK_UNQUOTED`, or `MASK_IFS`. Later stages (param expansion, tilde, IFS field split) must keep the mask in lockstep with the value string — `expand_mask()` is the helper for splicing. Breaking this invariant is the usual cause of expansion bugs.

### Signals

Three regimes set from `shell/shell_signal.c`: interactive prompt, heredoc read, and restored default. Switch regimes around anything that reads from the user or forks a child; don't install raw `signal()`/`sigaction()` calls in module code.

### Heredoc

`eval_heredoc` walks the AST **before** `eval` to collect heredoc bodies into pre-opened pipe fds (`t_command.heredoc_fds`). Expansion of `$VAR` inside heredoc bodies is gated on whether the delimiter was quoted.

## Subject constraints (42 minishell v10.0)

The subject (`minish.md`) is narrow about what external functions you may call. The full allow-list is: `readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs` plus everything from `libft`.

**Anything else is a grading failure**, including `fprintf`, `strcmp`, `strncmp`, `strlen`, `memset`, etc. Reach for the `ft_*` equivalent in `libft/`. (`srcs/main.c` currently has a stray `strcmp` call — treat it as a known violation to fix, not as precedent.)

Other hard rules worth keeping in mind:

- **At most one global variable**, and it may only hold the last received signal number. No structs in global scope. The singletons (`get_shell_data` / `get_shell_input`) use function-local `static`, which is compliant — don't "simplify" them into file-scope globals.
- Mandatory part must **not** interpret `\` or `;`. The tokenizer currently emits `TK_SEMICOLON` for bonus use; any change must keep `;` out of mandatory-mode behavior.
- Bonus features (`&&`, `||`, `(` `)`, `*` wildcards) must live in `_bonus.{c,h}` files and be gated behind the `bonus` Makefile rule. The current build compiles them unconditionally into the main binary and the `bonus:` rule is empty — this is a known compliance gap, not the intended design.
- `readline` itself is allowed to leak; **your code** must not leak.
- Bash is the reference implementation for any undefined behavior.
- Keep the scope tight — the subject explicitly says "Anything that is not asked is not required." The `-c` flag added in `srcs/main.c` is outside the subject; don't expand on non-subject features without an explicit ask.

## Conventions specific to this repo

- Headers live under `includes/` (flat, not per-module). Add prototypes to the header that matches the module directory name (e.g. `srcs/evaluation/*.c` → `includes/eval.h`).
- Each public header lists, in a banner comment, which `.c` file each prototype lives in — keep it current when moving functions.
- `get_next_line` is vendored (bonus variant) under `srcs/utils/` and exposed via `includes/get_next_line_bonus.h`; prefer it over re-implementing buffered reads.
- `t_exp_token { value; qmask; }` always travels as a pair — never expand one without the other.
- Error strings go to `STDERR_FILENO` via `ft_putstr_fd` / `ft_putendl_fd` to stay consistent with the rest of the codebase.

*This project has been created as part of the 42 curriculum by gbenetri, mguilber.*

# Minishell

## Description

Minishell is a simplified reimplementation of a Unix shell (bash), written in C.
It reads a command line, interprets it the way bash would, and executes it by
forking processes, wiring pipes and redirections, and handling built-in
commands directly. The goal of the project is to gain hands-on, low-level
understanding of processes, file descriptors, and signal handling — everything
a real shell relies on under the hood.

## Instructions

### Compilation

```
cd minishell
make
```

This produces a `minishell` executable in the project root. Available `Makefile`
rules:

| Rule      | Effect                                              |
|-----------|------------------------------------------------------|
| `make` / `make all` | Compiles the project                       |
| `make clean`        | Removes object files                       |
| `make fclean`       | Removes object files and the binary        |
| `make re`           | `fclean` then `all`                        |

The project is compiled with `cc`, `-Wall -Wextra -Werror`, and links against
`readline`.

### Running

```
./minishell
```

You get an interactive `minishell>` prompt, exactly like a normal shell. It can
also read commands from a pipe or a redirected file (non-interactive mode).

## Features

- Interactive prompt with a working command **history** (via `readline`).
- **Pipes** (`|`) chaining any number of commands.
- **Redirections**: `<`, `>`, `>>`, and heredocs (`<<`), including multiple
  heredocs on the same command line.
- **Quoting**: single quotes (`'...'`) suppress all expansion; double quotes
  (`"..."`) suppress everything except `$` expansion.
- **Environment variable expansion** (`$VAR`, `$?`) with word-splitting on
  unquoted results, matching bash's field-splitting behavior.
- **Signal handling** matching bash in interactive mode:
  - `Ctrl-C` interrupts the current line/command and redraws a clean prompt.
  - `Ctrl-D` exits the shell on an empty line.
  - `Ctrl-\` is ignored at the prompt, and terminates a running foreground
    command with the expected exit status.
- **Built-in commands**: `echo` (with `-n`), `cd` (relative or absolute path,
  plus `-`), `pwd`, `export`, `unset`, `env`, `exit`.
- PATH resolution for external commands, plus relative/absolute path execution
  (`./program`, `/bin/program`).

## Bonus part

Not implemented in this submission (`&&` / `||` with parentheses, and `*`
wildcard expansion).

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) —
  used as the reference behavior whenever the subject itself didn't specify
  something precisely.
- [GNU Readline Library documentation](https://tiswww.case.edu/php/chet/readline/readline.html)
- [42's `minishell_tester` by zstenger93](https://github.com/zstenger93/42_minishell_tester)
  and [`minishell_tester` by LucasKuhn](https://github.com/LucasKuhn/minishell_tester) —
  used to run large batches of behavioral tests against real bash.
- `man` pages for every authorized function listed above (`fork`, `execve`,
  `dup2`, `waitpid`, `signal`, `readline`, ...).


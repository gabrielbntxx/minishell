#ifndef MINISHELL_H
  #define MINISHELL_H

  # include <stdlib.h>
  # include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <stdio.h>
  #include <readline/readline.h>
# include <signal.h>


  #include "res.h"
  #include "env.h"
  #include "executor.h"
  #include "parser.h"
  #include "lexer.h"
  #include "builtins.h"
  #include "expand.h"
  extern int g_exit_st;
  extern int g_should_exit;

  void handler0(int sig);
  void update_exit(int status);
  void free_cmds(t_cmd *cmd);

#endif

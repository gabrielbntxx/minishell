#ifndef MINISHELL_H
  #define MINISHELL_H

  # include <stdlib.h>
  # include <unistd.h>
  #include <sys/wait.h>
  #include <stdio.h>
  #include <readline/readline.h>
# include <signal.h>
  #include <fcntl.h>


  #include "res.h"
  #include "env.h"
  #include "executor.h"
  #include "parser.h"
  #include "lexer.h"
  #include "builtins.h"

#endif

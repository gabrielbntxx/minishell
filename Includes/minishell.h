#ifndef MINISHELL_H
  #define MINISHELL_H

  # include <stdlib.h>
  # include <unistd.h>
  #include <sys/wait.h>
  #include <stdio.h>
  #include <readline/readline.h>
  #include "../src/res/res.h"
  #include "../src/env/env.h"
  #include "../src/executor/executor.h"
# include <signal.h>


  #include "res.h"
  #include "env.h"
  #include "executor.h"
  #include "parser.h"
  #include "lexer.h"
  #include "builtins.h"

#endif

#ifndef MINISHELL_H
  #define MINISHELL_H

  # include <stdlib.h>
  # include <unistd.h>
  #include <sys/wait.h>
  #include <stdio.h>
  #include <readline/readline.h>
  #include "res.h"
  #include "env.h"
  #include "executor.h"
  #include "parser.h"
  #include "lexer.h"
  #include "builtins.h"
// typedef struct s_env {
//   char *key;
//   char *value;
//   struct s_env *next;
// } t_env;


#endif

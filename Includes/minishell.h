#ifndef MINISHELL_H
  #define MINISHELL_H

  # include <stdlib.h>
  # include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <stdio.h>
  #include <readline/readline.h>
  #include <signal.h>


  #include "res.h"
  #include "env.h"
  #include "executor.h"
  #include "parser.h"
  #include "lexer.h"
  #include "builtins.h"
  #include "expand.h"
  extern int g_exit_st;

  void handler0(int sig);
  void update_exit(int status);
  void free_cmds(t_cmd *cmd);
  void free_tokens(t_token *tok);
  void free_cmds(t_cmd *cmd);
  void free_env(t_env *env);
  void free_all(t_token *tokens, t_cmd *cmds);
  int is_redir(int type);
  int syntax_error(char *token);
  int token_value_error(t_token *token);
  int check_pipe(t_token *cur);
  int check_redir(t_token *cur);

#endif

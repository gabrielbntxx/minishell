#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>


void super_exec(t_cmd *cmd, t_env *env) {
  char **array; 
  t_cmd *current;
  int *pipe_fd; // pipe_fd[0] = in, pipe_fd[1] = out,
  int last_fd; // save pour la prochaine commande
  int pid;

  current = cmd;
  array = env_to_array(env);
  if (current->next) {
    while (current) {
      if (current->next) pipe(pipe_fd);
      pid = fork();
        if (pid == 0) {
        if (last_fd != -1) dup2(last_fd, STDIN_FILENO);
        if (current->next) { 
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        execute_cmd(cmd, array);
        }
      }
      close(pipe_fd[1]);
      last_fd = pipe_fd[0];
      current = current->next;
    }
    while(wait(NULL) < 0);
  }
  else {
    if (dispatch(cmd, &env) == 1)
      execute_cmd(cmd, array);
  }
    
}

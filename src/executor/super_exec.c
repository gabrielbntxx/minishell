#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>

void apply_redir(t_cmd *cmd) {
  int fd[2];

  if(cmd->redir_in) {
    fd[0] = open(cmd->redir_in, O_RDONLY);
    dup2(fd[0], STDIN_FILENO);

    close(fd[0]);
  } 
  if(cmd->redir_out) {
    if (cmd->append == 0)
      fd[1] = open(cmd->redir_out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else
      fd[1] = open(cmd->redir_out, O_WRONLY | O_APPEND | O_CREAT, 0644);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
  }
  return;
}



// ici reside un truc misterieux

void super_exec(t_cmd *cmd, t_env *env) {
  char **array; 
  t_cmd *current;
  int pipe_fd[2]; // pipe_fd[0] = in, pipe_fd[1] = out,
  int last_fd = -1; // save pour la prochaine commande
  int pid; 

  current = cmd;
  array = env_to_array(env);

  if (current->next) {
    while (current) {
        if (current->next) pipe(pipe_fd);
        pid = fork();
      if (pid == 0) {
        if (last_fd != -1) { 
          dup2(last_fd, STDIN_FILENO);
          close(last_fd);
        } 
        if (current->next) { 
          dup2(pipe_fd[1], STDOUT_FILENO);
          close(pipe_fd[0]);
          close(pipe_fd[1]);
        }
        apply_redir(current);
        if (dispatch(current, &env) == 1)
          execute_cmd(current, array);
        exit(127);
      }
      if (current->next) {
        if (last_fd != -1) close(last_fd);
        close(pipe_fd[1]);
        last_fd = pipe_fd[0];
      }
      else close(last_fd);
      current = current->next;
    }
    while(wait(NULL) > 0);
  }
  else {
    if (pid == 0) {
      apply_redir(current);
      if (dispatch(cmd, &env) == 1)
      execute_cmd(cmd, array);
      exit(127); 
    }
  }
}

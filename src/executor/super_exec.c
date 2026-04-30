#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"

void super_exec(t_cmd *cmd, t_env *env) {
  char **array; 
  t_cmd *current;
  int *pipe_fd;
  int last_fd;
  int pid;

  current = cmd;
  array = env_to_array(env);
  if (current->next) {
    while (current) {
      
    }
  }
  else {
    if (dispatch(cmd, &env) == 1)
      execute_cmd(cmd, array);
  }
    
}

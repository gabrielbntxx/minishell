#include "../Includes/minishell.h"


int test() {
  printf("test\n");
}


int main(int ac, char **av, char **envp) {
  t_env *env;
  char *cmd;
  char **array;

  init_env(envp, &env);
  array = env_to_array(env);
  while (1) {
    cmd = readline("enter cmd -> ");
    execute_cmd(cmd, array);
  }
}

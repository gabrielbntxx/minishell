#include "../Includes/minishell.h"


static void    handler0(int sig)
{
    (void) sig;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}



int main(int ac, char **av, char **envp) {
  t_env *env;
  char *cmd;
  char **array;

  init_env(envp, &env);
  array = env_to_array(env);
  while (1) {
     signal(SIGINT, handler0); // gestion signaux
    signal(SIGQUIT, SIG_IGN); // gestion signaux
    cmd = readline("enter cmd -> ");
    if(!str) exit(0); 
    execute_cmd(cmd, array);
  }
}

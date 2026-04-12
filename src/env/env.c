
#include "../../Includes/minishell.h"
#include "../res/res.h"

char *env_get(t_env *env, char *key) {
  while (env->next != NULL) {
    ft_strcmp(env->key, key);
  }
}


void init_env(char **envp) {
  t_env env;
  int i;
  char *sep;
  int len;

  env.next = malloc(sizeof(t_env));

  while (envp[i]) {
    sep = ft_strchr(envp[i], '=');
    if (sep) {
    env.key = ft_substr(envp[i], 0, sep - envp[i]);
    env.value = ft_strdup(sep);
  }
  else
    env.key = ft_strdup(envp[i]);
  env = env.next;
  env.next = malloc(sizeof(t_env));
  i++;

  }
}

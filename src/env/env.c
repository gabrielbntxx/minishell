
#include "../../Includes/minishel.h"


char *env_get(t_env *env, char *key) {
  while (env->next != NULL) {
    ft_strcmp(env->key, key);
  }
}


t_env int_env(char **envp) {
  t_env env;
  int i;
  char *split;
  int len;

  i = 0;
  while (envp[i]) {
      split = ft_strchr(envp[i], '=');
      len = ft_strlen(envp);
      env.key = ft_substr(envp, 0, split - 1);
      env.value = ft_substr(envp, split + 1, len);
      i++;
  }
  env = NULL;
}


#include "../../Includes/minishell.h"
#include "../res/res.h"

char *env_get(t_env *env, char *key) {
  while (env->next != NULL) {
    ft_strcmp(env->key, key);
  }
}


void init_env(char **envp, t_env **first) {
  //t_env *first;
  t_env *node;
  t_env *current;
  int i = 0;
  int sep;
  int len;

  while (envp[i]) {
    node = malloc(sizeof(t_env));
    sep = ft_strchr(envp[i], '=');
    len = ft_strlen(envp[i]);
    node->next = NULL;
      if (sep != -1) {
        node->key = ft_substr(envp[i], 0, sep);
        node->value = ft_substr(envp[i], sep, len );
      }
      else {
        node->key = ft_strdup(envp[i]);
        node->value = NULL;
      }
      if (!*first) *first = node;
      else current->next = node;
      current = node;
      i++;
  }
  return;
}


int    main(int ac, char **av, char **envp)
{
  t_env *env;
  int i = 0;
  init_env(envp, &env);
  while (i < 4){
    printf("%s     %s\n", env->key, env->value);
    env = env->next;
    i++;
  }
}

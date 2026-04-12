
#include "../../Includes/minishell.h"
#include "../res/res.h"

char *env_get(t_env *env, char *key) {
  char *value;

  while (env) {
    if (ft_strcmp(env->key, key) == 0)
      return(env->value);
    env = env->next;
  }
  return (NULL);
}


void init_env(char **envp, t_env **first) {
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
        node->value = ft_substr(envp[i], sep + 1, len );
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
  char *value = env_get(env, "PWD");
  printf("%s\n", value);
  printf("%s\n", getenv("PWD"));
}

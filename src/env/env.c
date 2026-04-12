
#include "../../Includes/minishell.h"
#include "../res/res.h"



void *env_get(t_env *env, char *key, int option) {
  char *value;

  while (env) {
    if (ft_strcmp(env->key, key) == 0) {
      if (option == 1) return(env);
      return(env->value);
    }
    env = env->next;
  }
  if (option == 1) return(NULL);
  return ("dont exist\n");
}

void env_set(t_env **env, char *key, char *value) {
  t_env *node;

  node = env_get(*env, key, 1);
  if (!node) {
    while ((*env)->next != NULL) {
      *env = (*env)->next;
    }
    (*env)->next = malloc(sizeof(t_env));
    *env = (*env)->next;
    (*env)->key = key;
    (*env)->value = value;
    return;
  }
  (*env)->next = node;
  *env = node;
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
  char *value = env_get(env, "TEST", 0);
  printf("%s\n", value);
  env_set(&env, "TEST", "hello");
  value = env_get(env, "TEST", 0);
  printf("%s\n", value);
}

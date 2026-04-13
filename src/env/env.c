
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

void env_set(t_env **env, char *key, char *value)
{
    t_env *current;
    t_env *node;

    node = env_get(*env, key, 1);
    if (node)
    {
        free(node->value);
        node->value = ft_strdup(value);
        return ;
    }
    current = *env;
    while (current->next != NULL)
        current = current->next;
    current->next = malloc(sizeof(t_env));
    current = current->next;
    current->key = ft_strdup(key);
    current->value = ft_strdup(value);
    current->next = NULL;
}

void env_unset(t_env **env, char *key) {
  t_env *current;
  t_env *prev;

  current = *env;
  prev = NULL;
  while (current) {
    if (ft_strcmp(current->key, key) == 0) {
      if (prev)
        prev->next = current->next;
      else
        *env = current->next;
      free(current->key);
      free(current->value);
      free(current);
      break;
    }
    else {
    prev = current;
    }
    current = current->next;
  }
}

char **env_to_array(t_env *env) {
  char **array;
  t_env *current;
  int i;

  current = env;
  while (current) {
    i++;
    current = current->next;
  }
  array = malloc(sizeof(char *) * i);
  i = 0;
  current = env;
  while (current) {
    array[i] = ft_strjoin(ft_strjoin(current->key, "="), current->value);
    i++;
    current = current->next;
  }
  array[i] = NULL;
  return (array);
}

void env_free(t_env *env) {
  t_env *prev;
  while (env) {
    free(env->key);
    free(env->value);
    prev = env;
    env = env->next;
    free(prev);
  }
}

void init_env(char **envp, t_env **first) {
  t_env *node;
  t_env *current;
  int i = 0;
  int sep;
  int len;

  current = NULL;
  while (envp[i]) {
    node = malloc(sizeof(t_env));
    if (!node)  {
      env_free(*first);
      return;
    }
    sep = ft_strchr(envp[i], '=');
    len = ft_strlen(envp[i]);
    node->next = NULL;
      if (sep != -1) {
        node->key = ft_substr(envp[i], 0, sep);
        node->value = ft_substr(envp[i], sep + 1, len - sep - 1);
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
  env = NULL;
  init_env(envp, &env);

  char *value = env_get(env, "TEST", 0);
  printf("exist pas -> %s\n", value);

  env_set(&env, "TEST", "hello");

  value = env_get(env, "TEST", 0);
  printf("existe -> %s\n", value);

  env_unset(&env, "TEST");

  value = env_get(env, "TEST", 0);
  printf("supp -> %s\n\n", value);


  // char **array = env_to_array(env);
  // while (array[i]) {
  //   printf("%s\n", array[i]);
  //   i++;
  // }


    //env_set(&env, "TEST", "hello");

    value = env_get(env, "TEST", 0);
    printf("existe -> %s\n", value);


      // array = env_to_array(env);
      // while (array[i]) {
      //   printf("%s\n", array[i]);
      //   i++;
      // }

      env_free(env);

  // while (i < 9){
  //   printf("%s     %s\n", env->key, env->value);
  //   env = env->next;
  //   i++;
  // }
}

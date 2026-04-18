#include "builtins.h"
char	*ft_strjoin(const char *s1, const char *s2);

#include <stdio.h>
#include <string.h>
#include <string.h>

char **sort_array(char **env) {
    if (!env) return (env);

    int n = 0;
    while (env[n])
        n++;

    int pass = 0;
    while (pass < n - 1) {
        int i = 0;
        int swapped = 0;
        while (i < n - 1 - pass) {
            if (strcmp(env[i], env[i + 1]) > 0) {
                char *tmp = env[i];
                env[i] = env[i + 1];
                env[i + 1] = tmp;
                swapped = 1;
            }
            i++;
        }
        if (!swapped) break;
        pass++;
    }
    return env;
}



char **env_to_export(t_env *env) {
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
    array[i] = ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin(current->key, "="), "\""), current->value), "\"");
    i++;
    current = current->next;
  }
  array[i] = NULL;
  return (array);
}

void print_export(char **env) {

  int i = 0;
    char *tmp;
    char *decla = "declare -x ";


    while(env[i]) {
        tmp = ft_strjoin(decla, env[i]);
        printf("%s\n", tmp);
        i++;
    }
    return;
}

void ft_export(char **cmd, t_env *nodenv) {
  int i = 0;
  char **env;
  char *key;
  char *value;
  int len;
  env = env_to_export(nodenv);
  env = sort_array(env);
  if (!cmd[1]) {
    print_export(env);
    return;
  }
  while (cmd[i]) {
    sep = ft_strchr(envp[i], '=');
    len = ft_strlen(envp[i]);
    if (sep != -1) {
      key = ft_substr(envp[i], 0, sep);
      value = ft_substr(envp[i], sep + 1, len - sep - 1);
    }
    else {
      key = ft_strdup(envp[i]);
        value = NULL;
    }
  }
}

int main(int ac, char **av, char **envp) {
  t_env *nodenv;
  char **cmd[0] = export;
  char **cmd[1] = "test4=icilaba";
  init_env(envp, &nodenv);
  ft_export(cmd, nodenv);
}

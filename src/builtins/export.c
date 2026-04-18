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

void ft_export(char **env) {
    char **sorted;
    int i = 0;
    //sorted = env_to_array(env);
    sorted = sort_array(sorted);
    print_export(env);
}

int main(int ac, char **av, char **envp) {
    int i = 0;
    char **env;
    t_env *nodenv;
    init_env(envp, &nodenv);
    env = env_to_export(nodenv);
    //while (envp[])
    env = sort_array(env);
    print_export(env);
}

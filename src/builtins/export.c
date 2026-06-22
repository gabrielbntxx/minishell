/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:13 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/11 13:23:42 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
char	*ft_strjoin(const char *s1, const char *s2);

#include <stdio.h>
#include <string.h>
#include <string.h>

char **sort_array(char **env) {
    int n;
    int i; 
    char *tmp; 
    int swapped;
    int pass; 

    if (!env) return (env);

    n = 0;
    while (env[n])
        n++;

    pass = 0;
    while (pass < n - 1) {
        i = 0;
        swapped = 0;
        while (i < n - 1 - pass) {
            if (ft_strcmp(env[i], env[i + 1]) > 0) {
                tmp = env[i];
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

static char *super_join(t_env *current) {
  char *tmp;
  char *tmp2;
  char *array;

   if (current->value) {
      tmp = ft_strjoin( current->key, "=");
      tmp2 = ft_strjoin(tmp, "\"");
      free(tmp);
      tmp = ft_strjoin(tmp2, current->value);
      free(tmp2);
      array = ft_strjoin(tmp, "\"");
      free(tmp);
   }
    else
      array = ft_strjoin(current->key, NULL);
    return (array);
}

char **env_to_export(t_env *env) {
  char **array;
  t_env *current;
  int i = 0;

  current = env;
  while (current) {
    i++;
    current = current->next;
  }
  array = malloc(sizeof(char *) * i + 8);
  if (!array) return (NULL); 
  i = 0;
  current = env;
  while (current) {
    if (current->value)
      array[i] = super_join(current);
    else 
     array[i] = ft_strdup(current->key);
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
        free(tmp);
        i++;
    }
    return;
}
int is_valid(char **cmd)
{
    int i;
    int j;

    i = 0;
    while (cmd[i])
    {
        j = 0;
        if (!(cmd[i][j] == '_' || (cmd[i][j] >= 'A' && cmd[i][j] <= 'Z') || (cmd[i][j] >= 'a' && cmd[i][j] <= 'z')))
        {
            printf("not a valid identifier\n");
            return 0;
        }
        j++;
        while (cmd[i][j] && cmd[i][j] != '=')
        {
            if (!(cmd[i][j] == '_' || (cmd[i][j] >= 'A' && cmd[i][j] <= 'Z') || (cmd[i][j] >= 'a' && cmd[i][j] <= 'z') || (cmd[i][j] >= '0' && cmd[i][j] <= '9')))
            {
                printf("not a valid identifier\n");
                return 0;
            }
            j++;
        }
        i++;
    }
    return 1;
}

void builtin_export(char **cmd, t_env **nodenv) {
  int i = 0;
  char **env;
  char *key;
  char *value;
  int sep;
  int len;
  env = env_to_export(*nodenv);
  env = sort_array(env);
  if (!cmd[1]) {
    print_export(env);
    free_array(env);
    return;
  }
    if (is_valid(cmd) == 0) free_array(env);
    if (is_valid(cmd) == 0) return;
  while (cmd[++i]) {
    sep = ft_strchr(cmd[i], '=');
    len = ft_strlen(cmd[i]);
    if (sep != -1) {
      key = ft_substr(cmd[i], 0, sep);
      value = ft_substr(cmd[i], sep + 1, len - sep - 1);
      env_set(nodenv, key, value);
      free(key);
      free(value);
    }
    else {
      key = ft_strdup(cmd[i]);
      env_set(nodenv, key, NULL);
      free(key);
    }
  }
  free_array(env);
  return;
}



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

int is_valid( char *cmd)
{
  int i = 0;

  if (!cmd || !cmd[0])
    return 0;

  if (!((cmd[0] >= 'A' && cmd[0] <= 'Z') ||
    (cmd[0] >= 'a' && cmd[0] <= 'z') ||
    cmd[0] == '_'))
  {
    write(2, "minishell: export: `", 20);
    write(2, cmd, ft_strlen(cmd));
    write(2, "': not a valid identifier\n", 26);
      return 0;
  }
  i = 1;
  while (cmd[i])
  {
    if (!((cmd[i] >= 'A' && cmd[i] <= 'Z') ||
      (cmd[i] >= 'a' && cmd[i] <= 'z') ||
      (cmd[i] >= '0' && cmd[i] <= '9') ||
      cmd[i] == '_')) 
    {
        //write(1, "not a valide identifier\n", 25);
        return 0;
    }
    i++;
  }
  return 1;
}

int builtin_export(char **cmd, t_env **nodenv) {
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
    return 1;
  }
   
  while (cmd[++i]) {
    sep = ft_strchr(cmd[i], '=');
    len = ft_strlen(cmd[i]);
    if (sep != -1) {
      key = ft_substr(cmd[i], 0, sep);
      if (is_valid(key) == 0) { 
        free_array(env);
        free(key);
        return 1;
      }
      value = ft_substr(cmd[i], sep + 1, len - sep - 1);
      env_set(nodenv, key, value);
      free(key);
      free(value);
    }
    else {
      key = ft_strdup(cmd[i]);
      if (env_get(*nodenv, key, 0) || is_valid(key) == 0) {
        free(key);
        free_array(env);
        return 1;
      }
      env_set(nodenv, key, NULL);
      free(key);
    }
  }
  free_array(env);
  return 0;
}



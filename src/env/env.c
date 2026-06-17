/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:23 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/11 14:08:32 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Includes/minishell.h"






static char *super_join(t_env *current) {
  char *tmp;
  char *array;

   if (current->value) {
      tmp = ft_strjoin( current->key, "=");
      array = ft_strjoin(tmp, current->value);
      free(tmp);
   }
    else
      array = ft_strjoin(current->key, NULL);
  return(array);
}

char **env_to_array(t_env *env) {
  char **array;
  t_env *current;
  int i;

  i = 0;
  current = env;
  while (current) {
    i++;
    current = current->next;
  }
  array = malloc(sizeof(char **) * (i + 1));
  if (!array) {
    return (NULL);
  }
  i = 0;
  current = env;
  while (current) {
    array[i] = super_join(current);
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

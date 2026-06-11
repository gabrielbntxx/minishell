/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envUtils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 15:15:28 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/11 13:03:06 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void *env_get(t_env *env, char *key, int option) {
  while (env) {
    if (ft_strcmp(env->key, key) == 0) {
      if (option == 1) return(env);
      return(env->value);
    }
    env = env->next;
  }
  if (option == 1) return(NULL);
  return ("");
}

static void exist_node(t_env *node, char *value) {

    free(node->value);
        if (value)
          node->value = ft_strdup(value);
        else
          node->value = NULL;
        return ;
}


void env_set(t_env **env, char *key, char *value)
{
    t_env *current;
    t_env *node;

    node = env_get(*env, key, 1);
    if (node)    
        exist_node(node, value);
    if (node)
        return;
    current = *env;
    if (current) {
      while (current->next != NULL)
          current = current->next;
      current->next = malloc(sizeof(t_env));
      if (!current->next) 
       return;
      current = current->next;
    }
    if (!key) 
        return;
      current->key = ft_strdup(key);
    if (value)
      current->value = ft_strdup(value);
    else
      current->value = NULL;
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
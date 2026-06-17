/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:26:35 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/11 14:00:14 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"



int dispatch(t_cmd *cmd, t_env **env) {
    char *this;
    this = cmd->args[0];
    char **envp = env_to_array(*env);
  
  
    if (!ft_strcmp(this, "export")) 
        builtin_export(cmd->args, env);
    else if (!ft_strcmp(this, "env")) builtin_env(envp);
    else if (!ft_strcmp(this, "pwd")) builtin_pwd();
    else if (!ft_strcmp(this, "cd")) builtin_cd(*env, cmd->args); 
    else if (!ft_strcmp(this, "echo")) builtin_echo(cmd->args);
    else if (!ft_strcmp(this, "exit")) { 
      free_array(envp); 
      g_exit_st = builtin_exit(cmd->args, g_exit_st);
      return (g_exit_st);
    }
    else if (!ft_strcmp(this, "unset")) builtin_unset(cmd->args, *env);
    else {
        free_array(envp);
        return (1);
    }
    free_array(envp);
    return (0);
}

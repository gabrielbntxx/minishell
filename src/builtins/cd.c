/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:26:45 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/08 15:12:17 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"

int builtin_cd(t_env *env, char **args)
{
    char *path;


    if (args[1] == NULL)
    {
        path = env_get(env, "HOME", 0);
        if (path == NULL)
        {
            write(2, "minishell: cd: HOME not set\n", 28);
            return(1);
        } 
    }
    else
      path = args[1];
    if (args[1]) {
      if (!ft_strcmp(args[1], "-")) {
        path = env_get(env, "OLDPWD", 0);
        if (path == NULL){
          printf("cd: OLDPWD not set");
          return (1);
       }
      }
    }
    char    buf[4096];
    char    *cwd;

    if (chdir(path) == -1)
    {
        perror("minishell: cd");
        return(1);
    }
    env_set(&env, "OLDPWD", env_get(env, "PWD", 0));
    cwd = getcwd(buf, sizeof(buf));
    env_set(&env, "PWD", cwd ? cwd : path);
    return(0);
}

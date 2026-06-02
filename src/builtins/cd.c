/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:26:45 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/02 20:26:45 by mguilber         ###   ########.fr       */
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
    {
        path = args[1];
    }
    if (chdir(path) == -1)
    {
        perror("minishell: cd");
        return(1);
    }
    return(0);
}

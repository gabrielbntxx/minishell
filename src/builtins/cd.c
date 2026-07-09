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

#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"
#include "../../Includes/parser.h"

int	builtin_cd(t_env *env, char **args)
{
	char	*path;

	if (args[1] == NULL)
	{
		path = env_get(env, "HOME", 0);
		if (path == NULL)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else
		path = args[1];
	if (args[1] && !ft_strcmp(args[1], "-"))
	{
		path = env_get(env, "OLDPWD", 0);
		if (!path || !path[0])
		{
			write(2, "minishell: cd: OLDPWD not set\n", 30);
			return (1);
		}
	}
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	env_set(&env, "OLDPWD", env_get(env, "PWD", 0));
	env_set(&env, "PWD", path);
	return (0);
}

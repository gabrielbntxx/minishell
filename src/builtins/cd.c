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

static char	*cd_target(t_env *env, char **args)
{
	char	*path;

	if (args[1] == NULL || !ft_strcmp(args[1], "--"))
	{
		path = env_get(env, "HOME", 0);
		if (path == NULL)
			write(2, "minishell: cd: HOME not set\n", 28);
		return (path);
	}
	if (!ft_strcmp(args[1], "-"))
	{
		path = env_get(env, "OLDPWD", 0);
		if (!path || !path[0])
		{
			write(2, "minishell: cd: OLDPWD not set\n", 30);
			return (NULL);
		}
		ft_putstr_fd(path, 1);
		write(1, "\n", 1);
		return (path);
	}
	return (args[1]);
}

int	builtin_cd(t_env *env, char **args)
{
	char	*path;
	char	buff[1024];

	path = cd_target(env, args);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		write(2, "minishell: cd: ", 15);
		perror(path);
		return (1);
	}
	env_set(&env, "OLDPWD", env_get(env, "PWD", 0));
	if (getcwd(buff, 1024) != NULL)
		env_set(&env, "PWD", buff);
	return (0);
}

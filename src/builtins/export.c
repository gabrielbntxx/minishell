/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:13 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	opt_error(char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': invalid option\n", 18);
	return (2);
}

static int	export_flag(char *arg, t_env **nodenv)
{
	if (is_valid(arg) == 0)
		return (1);
	env_get(*nodenv, arg, 1);
	return (0);
}

static int	export_kv(char *arg, int sep, t_env **nodenv)
{
	char	*key;
	char	*value;

	key = ft_substr(arg, 0, sep);
	if (is_valid(key) == 0)
	{
		free(key);
		return (1);
	}
	value = ft_substr(arg, sep + 1, ft_strlen(arg) - sep - 1);
	env_set(nodenv, key, value);
	free(key);
	free(value);
	return (0);
}

static int	export_one(char *arg, t_env **nodenv)
{
	int	sep;

	if (arg[0] == '-' && arg[1])
		return (opt_error(arg));
	sep = ft_strchr(arg, '=');
	if (sep > 0 && arg[sep - 1] == '+')
		return (export_append(arg, sep, nodenv));
	if (sep == -1)
		return (export_flag(arg, nodenv));
	return (export_kv(arg, sep, nodenv));
}

int	builtin_export(char **cmd, t_env **nodenv)
{
	int		i;
	int		r;
	int		ret;
	char	**env;

	if (!cmd[1])
	{
		env = sort_array(env_to_export(*nodenv));
		print_export(env);
		free_array(env);
		return (0);
	}
	ret = 0;
	i = 0;
	while (cmd[++i])
	{
		r = export_one(cmd[i], nodenv);
		if (r == 2)
			return (2);
		if (r == 1)
			ret = 1;
	}
	return (ret);
}

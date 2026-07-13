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

#include "../../Includes/minishell.h"

char	**sort_array(char **env)
{
	int		n;
	int		i;
	char	*tmp;
	int		swapped;
	int		pass;

	if (!env)
		return (env);
	n = 0;
	while (env[n])
		n++;
	pass = 0;
	while (pass < n - 1)
	{
		i = 0;
		swapped = 0;
		while (i < n - 1 - pass)
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
		if (!swapped)
			break ;
		pass++;
	}
	return (env);
}

static char	*super_join(t_env *current)
{
	char	*tmp;
	char	*tmp2;
	char	*array;

	if (current->value)
	{
		tmp = ft_strjoin(current->key, "=");
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

char	**env_to_export(t_env *env)
{
	char	**array;
	t_env	*current;
	int		i;

	i = 0;
	current = env;
	while (current)
	{
		i++;
		current = current->next;
	}
	array = malloc(sizeof(char *) * (i + 8));
	if (!array)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
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

void	print_export(char **env)
{
	int		i;
	char	*tmp;
	char	*decla;

	i = 0;
	decla = "declare -x ";
	while (env[i])
	{
		tmp = ft_strjoin(decla, env[i]);
		ft_putstr_fd(tmp, 1);
		write(1, "\n", 1);
		free(tmp);
		i++;
	}
	return ;
}

static int	invalid_msg(char *cmd)
{
	write(2, "minishell: export: `", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "': not a valid identifier\n", 26);
	return (0);
}

int	is_valid(char *cmd)
{
	int	i;

	if (!cmd)
		return (0);
	if (!cmd[0] || (!(cmd[0] >= 'A' && cmd[0] <= 'Z')
			&& !(cmd[0] >= 'a' && cmd[0] <= 'z')
			&& cmd[0] != '_'))
		return (invalid_msg(cmd));
	i = 1;
	while (cmd[i])
	{
		if (!((cmd[i] >= 'A' && cmd[i] <= 'Z')
				|| (cmd[i] >= 'a' && cmd[i] <= 'z')
				|| (cmd[i] >= '0' && cmd[i] <= '9')
				|| cmd[i] == '_'))
			return (invalid_msg(cmd));
		i++;
	}
	return (1);
}

static int	export_append(char *arg, int sep, t_env **nodenv)
{
	char	*key;
	char	*value;
	char	*old;
	char	*joined;

	key = ft_substr(arg, 0, sep - 1);
	if (is_valid(key) == 0)
	{
		free(key);
		return (1);
	}
	value = ft_substr(arg, sep + 1, ft_strlen(arg) - sep - 1);
	old = env_get(*nodenv, key, 0);
	if (old)
	{
		joined = ft_strjoin(old, value);
		env_set(nodenv, key, joined);
		free(joined);
	}
	else
		env_set(nodenv, key, value);
	free(key);
	free(value);
	return (0);
}

static int	export_one(char *arg, t_env **nodenv)
{
	int		sep;
	char	*key;
	char	*value;

	sep = ft_strchr(arg, '=');
	if (sep > 0 && arg[sep - 1] == '+')
		return (export_append(arg, sep, nodenv));
	if (sep == -1)
	{
		if (is_valid(arg) == 0)
			return (1);
		if (!env_get(*nodenv, arg, 1))
			env_set(nodenv, arg, NULL);
		return (0);
	}
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

int	builtin_export(char **cmd, t_env **nodenv)
{
	int		i;
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
		if (export_one(cmd[i], nodenv))
			ret = 1;
	}
	return (ret);
}

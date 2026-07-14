/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

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

int	export_append(char *arg, int sep, t_env **nodenv)
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

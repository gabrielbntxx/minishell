/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:20 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/11 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	unset_valid(char *arg)
{
	int	i;

	if (!arg[0] || (!(arg[0] >= 'A' && arg[0] <= 'Z')
			&& !(arg[0] >= 'a' && arg[0] <= 'z') && arg[0] != '_'))
		return (0);
	i = 1;
	while (arg[i])
	{
		if (!((arg[i] >= 'A' && arg[i] <= 'Z')
				|| (arg[i] >= 'a' && arg[i] <= 'z')
				|| (arg[i] >= '0' && arg[i] <= '9')
				|| arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	unset_error(char *arg)
{
	write(2, "minishell: unset: `", 19);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
}

int	builtin_unset(char **args, t_env **env)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1])
		{
			write(2, "minishell: unset: `", 19);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': invalid option\n", 18);
			return (2);
		}
		if (!unset_valid(args[i]))
		{
			unset_error(args[i]);
			ret = 1;
		}
		else
			env_unset(env, args[i]);
		i++;
	}
	return (ret);
}

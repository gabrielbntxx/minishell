/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	bubble_pass(char **env, int limit)
{
	int		i;
	int		swapped;
	char	*tmp;

	i = 0;
	swapped = 0;
	while (i < limit - 1)
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
	return (swapped);
}

char	**sort_array(char **env)
{
	int	n;
	int	pass;

	if (!env)
		return (env);
	n = 0;
	while (env[n])
		n++;
	pass = 0;
	while (pass < n - 1 && bubble_pass(env, n - pass))
		pass++;
	return (env);
}

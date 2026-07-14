/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:23 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static char	*super_join(t_env *current)
{
	char	*tmp;
	char	*array;

	if (current->value)
	{
		tmp = ft_strjoin(current->key, "=");
		array = ft_strjoin(tmp, current->value);
		free(tmp);
	}
	else
		array = ft_strjoin(current->key, NULL);
	return (array);
}

static int	env_len(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**env_to_array(t_env *env)
{
	char	**array;
	t_env	*current;
	int		i;

	array = malloc(sizeof(char *) * (env_len(env) + 1));
	if (!array)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		if (current->value)
			array[i++] = super_join(current);
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

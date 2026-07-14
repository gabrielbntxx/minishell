/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

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

static char	*escape_value(char *value)
{
	char	*out;
	int		i;
	int		j;

	out = malloc(ft_strlen(value) * 2 + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\' || value[i] == '$')
			out[j++] = '\\';
		out[j++] = value[i++];
	}
	out[j] = '\0';
	return (out);
}

static char	*super_join(t_env *current)
{
	char	*tmp;
	char	*tmp2;
	char	*escaped;
	char	*array;

	if (current->value)
	{
		escaped = escape_value(current->value);
		tmp = ft_strjoin(current->key, "=");
		tmp2 = ft_strjoin(tmp, "\"");
		free(tmp);
		tmp = ft_strjoin(tmp2, escaped);
		free(tmp2);
		free(escaped);
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

	array = malloc(sizeof(char *) * (env_len(env) + 8));
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

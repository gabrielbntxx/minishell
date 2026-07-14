/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	env_free(t_env *env)
{
	t_env	*prev;

	while (env)
	{
		free(env->key);
		free(env->value);
		prev = env;
		env = env->next;
		free(prev);
	}
}

static t_env	*make_node(char *entry)
{
	t_env	*node;
	int		sep;
	int		len;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	sep = ft_strchr(entry, '=');
	len = ft_strlen(entry);
	node->next = NULL;
	if (sep != -1)
	{
		node->key = ft_substr(entry, 0, sep);
		node->value = ft_substr(entry, sep + 1, len - sep - 1);
	}
	else
	{
		node->key = ft_strdup(entry);
		node->value = NULL;
	}
	return (node);
}

static void	bump_shlvl(t_env **first)
{
	char	*cur;
	char	*next;
	int		n;
	int		i;

	cur = env_get(*first, "SHLVL", 0);
	n = 0;
	i = 0;
	if (cur)
		while (cur[i] >= '0' && cur[i] <= '9')
			n = n * 10 + (cur[i++] - '0');
	next = ft_itoa(n + 1);
	env_set(first, "SHLVL", next);
	free(next);
}

void	init_env(char **envp, t_env **first)
{
	t_env	*node;
	t_env	*current;
	int		i;

	i = 0;
	current = NULL;
	while (envp[i])
	{
		node = make_node(envp[i]);
		if (!node)
		{
			env_free(*first);
			return ;
		}
		if (!*first)
			*first = node;
		else
			current->next = node;
		current = node;
		i++;
	}
	bump_shlvl(first);
}

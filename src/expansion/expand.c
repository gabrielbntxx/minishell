/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:34 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/17 18:25:17 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/expand.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (8);
	return (0);
}

static char	*get_expand_value(char *arg, int y, int *end, t_env *env)
{
	char	*name;
	char	*env_value;
	char	*value;

	if (arg[y + 1] == '?')
	{
		*end = y + 2;
		return (ft_itoa(g_exit_st));
	}
	while (arg[*end] && ft_isalnum(arg[*end]))
		(*end)++;
	if (*end == y + 1)
		return (NULL);
	name = ft_substr(arg, y + 1, *end - (y + 1));
	env_value = env_get(env, name, 0);
	if (env_value)
		value = ft_strdup(env_value);
	else
		value = ft_strdup("");
	free(name);
	return (value);
}

static int	replace_expand(char **arg, int y, int end, char *value)
{
	char	*tmp;
	char	*post;
	char	*mid;
	char	*old;

	tmp = ft_substr(*arg, 0, y);
	post = ft_substr(*arg, end, ft_strlen(*arg) - end);
	mid = ft_strjoin(tmp, value);
	old = *arg;
	*arg = ft_strjoin(mid, post);
	free(old);
	free(tmp);
	free(post);
	free(mid);
	free(value);
	if (!*arg)
		return (1);
	return (0);
}

static int	expand_one_arg(char **arg, t_env *env)
{
	int		y;
	int		end;
	char	*value;

	while (ft_strchr(*arg, '$') != -1)
	{
		y = ft_strchr(*arg, '$');
		end = y + 1;
		value = get_expand_value(*arg, y, &end, env);
		if (!value)
			break ;
		if (replace_expand(arg, y, end, value))
			return (1);
	}
	return (0);
}

void	expand(t_cmd *cmd, t_env *env)
{
	int	i;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (!cmd->args_quote || cmd->args_quote[i] != SINGLE) 
		{
			if (expand_one_arg(&cmd->args[i], env))
				return ;
		}
		i++;
	}
}

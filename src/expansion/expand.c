/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:34 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/08 22:04:35 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/expand.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_')
		return (8);
	return (0);
}

static char	*get_expand_value(char *arg, int y, int *end, t_shell *sh)
{
	char	*name;
	char	*env_value;
	char	*value;

	if (arg[y + 1] == '?')
	{
		*end = y + 2;
		return (ft_itoa(sh->status));
	}
	while (arg[*end] && ft_isalnum(arg[*end]))
		(*end)++;
	if (*end == y + 1)
		return (NULL);
	name = ft_substr(arg, y + 1, *end - (y + 1));
	env_value = env_get(*sh->env, name, 0);
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

static int	expnd_pos(char **arg, int *start, int y, t_shell *sh)
{
	int		end;
	int		len;
	char	*value;

	end = y + 1;
	value = get_expand_value(*arg, y, &end, sh);
	if (!value)
	{
		(*start)++;
		return (0);
	}
	len = ft_strlen(value);
	if (replace_expand(arg, y, end, value))
	{
		free(*arg);
		return (1);
	}
	*start = y + len;
	return (0);
}

int	expand_one_arg(char **arg, t_shell *sh)
{
	int	start;
	int	y;

	start = 0;
	while (ft_strchr(*arg + start, '$') != -1)
	{
		y = start + ft_strchr(*arg + start, '$');
		if (expnd_pos(arg, &start, y, sh))
			return (1);
	}
	return (0);
}

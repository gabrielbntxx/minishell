/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:34 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/02 20:54:35 by mguilber         ###   ########.fr       */
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
		return (ft_itoa(sh->last_status));
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

static int	expand_one_arg(char **arg, t_shell *sh)
{
	int		y;
	int		end;
	char	*value;

	while (ft_strchr(*arg, '$') != -1)
	{
		y = ft_strchr(*arg, '$');
		end = y + 1;
		value = get_expand_value(*arg, y, &end, sh);
		if (!value)
			break ;
		if (replace_expand(arg, y, end, value))
		{
			free(*arg);
			return (1);
		}
	}
	return (0);
}

void	expand(t_cmd *cmd, t_shell *sh)
{
	int	i;

	i = 0;
	if (cmd->redir_in && ft_strchr(cmd->redir_in, '$'))
		expand_one_arg(&cmd->redir_in, sh);
	if (cmd->redir_out && ft_strchr(cmd->redir_out, '$'))
		expand_one_arg(&cmd->redir_out, sh);
	if (cmd->heredoc && ft_strchr(cmd->heredoc, '$'))
		expand_one_arg(&cmd->heredoc, sh);
	while (cmd->args && cmd->args[i])
	{
		if (!(cmd->args_quote[i] == SINGLE))
			if (expand_one_arg(&cmd->args[i], sh))
				return ;
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:39 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/08 21:55:20 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	word_fixups(t_token *tok, t_env **env, int start)
{
	char	*home;
	char	*joined;
	int		len;

	len = ft_strlen(tok->value);
	if (len > 0 && tok->value[len - 1] == '$' && tok->quote_type == NONE
		&& tok->no_space && tok->next && tok->next->type == WORD
		&& tok->next->quote_type != NONE)
		tok->value[len - 1] = '\0';
	if (!start || tok->quote_type != NONE || tok->value[0] != '~'
		|| (tok->value[1] && tok->value[1] != '/'))
		return ;
	home = env_get(*env, "HOME", 0);
	if (!home)
		return ;
	joined = ft_strjoin(home, tok->value + 1);
	if (!joined)
		return ;
	free(tok->value);
	tok->value = joined;
}

void	expand_tokens(t_token *tok, t_shell *sh)
{
	int	skip;
	int	start;

	skip = 0;
	start = 1;
	while (tok)
	{
		if (tok->type != WORD)
			skip = (tok->type == HEREDOC);
		else
		{
			if (!skip)
				word_fixups(tok, sh->env, start);
			if (!skip && tok->quote_type != SINGLE)
        expand_one_arg(&tok->value, sh);
			skip = (skip && tok->no_space);
			start = !tok->no_space;
		}
		if (tok->type != WORD)
			start = 1;
		tok = tok->next;
	}
}

static void	merge_pair(t_token *tok)
{
	t_token	*next;
	char	*joined;

	next = tok->next;
	joined = ft_strjoin(tok->value, next->value);
	free(tok->value);
	tok->value = joined;
	if (next->quote_type != NONE)
		tok->quote_type = SINGLE;
	tok->no_space = next->no_space;
	tok->next = next->next;
	free(next->value);
	free(next);
}

void	merge_tokens(t_token *tok)
{
	while (tok)
	{
		if (tok->type == WORD && tok->no_space
			&& tok->next && tok->next->type == WORD)
			merge_pair(tok);
		else
			tok = tok->next;
	}
}

char	*read_line_notty(void)
{
	char	buf[4096];
	int		i;
	int		r;

	i = 0;
	r = 1;
	while (i < 4095)
	{
		r = read(STDIN_FILENO, buf + i, 1);
		if (r <= 0 || buf[i] == '\n')
			break ;
		i++;
	}
	if (i == 0 && r <= 0)
		return (NULL);
	buf[i] = '\0';
	return (ft_strdup(buf));
}

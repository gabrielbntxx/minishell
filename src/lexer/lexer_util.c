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


void	expand_tokens(t_token *tok, t_env **env)
{
	int	skip;

	skip = 0;
	while (tok)
	{
		if (tok->type != WORD)
			skip = (tok->type == HEREDOC);
		else
		{
			if (!skip && tok->quote_type != SINGLE)
				expand_one_arg(&tok->value, env);
			skip = (skip && tok->no_space);
		}
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


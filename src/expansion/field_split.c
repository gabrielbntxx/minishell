/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 00:00:00 by gbenetri          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	needs_split(t_token *tok)
{
	int	i;

	if (tok->type != WORD || tok->quote_type != NONE)
		return (0);
	i = 0;
	while (tok->value[i])
	{
		if (is_ifs(tok->value[i]))
			return (1);
		i++;
	}
	return (0);
}

static t_token	*split_value(char *value)
{
	t_token	*head;
	char	*val;
	int		i;
	int		start;

	head = NULL;
	i = 0;
	while (value[i])
	{
		while (value[i] && is_ifs(value[i]))
			i++;
		start = i;
		while (value[i] && !is_ifs(value[i]))
			i++;
		if (i > start)
		{
			val = ft_substr(value, start, i - start);
			add_token(&head, new_token(WORD, val, NONE, 0));
			free(val);
		}
	}
	return (head);
}

static t_token	*splice_pieces(t_token **head, t_token *prev, t_token *cur,
		t_token *next)
{
	t_token	*pieces;
	t_token	*tail;

	pieces = split_value(cur->value);
	if (!pieces)
		pieces = new_token(WORD, "", NONE, 0);
	if (prev)
		prev->next = pieces;
	else
		*head = pieces;
	tail = pieces;
	while (tail->next)
		tail = tail->next;
	tail->no_space = cur->no_space;
	tail->next = next;
	free(cur->value);
	free(cur);
	return (tail);
}

void	field_split(t_token **head)
{
	t_token	*prev;
	t_token	*cur;
	t_token	*next;

	prev = NULL;
	cur = *head;
	while (cur)
	{
		next = cur->next;
		if (needs_split(cur))
			prev = splice_pieces(head, prev, cur, next);
		else
			prev = cur;
		cur = next;
	}
}

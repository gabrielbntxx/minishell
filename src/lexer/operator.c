/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:47 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"

int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_operator(char c)
{
	if (c != '<' && c != '>' && c != '|')
	{
		return (0);
	}
	return (1);
}

static int	add_op(t_token **head, t_token_type type, char *val, int len)
{
	add_token(head, new_token(type, val, NONE, 0));
	return (len);
}

int	get_operator_token(char *input, int *i, t_token **head)
{
	if (input[*i] == '|')
		return (add_op(head, PIPE, "|", 1));
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (add_op(head, APPEND, ">>", 2));
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (add_op(head, HEREDOC, "<<", 2));
	if (input[*i] == '>')
		return (add_op(head, REDIR_OUT, ">", 1));
	if (input[*i] == '<')
		return (add_op(head, REDIR_IN, "<", 1));
	return (0);
}

int	handle_quote(char *input, int *i, t_token **head, t_quote_type qt)
{
	int		start;
	int		no_space;
	char	*word;
	char	q;

	q = '\'';
	if (qt == DOUBLE)
		q = '"';
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != q)
		(*i)++;
	if (input[*i] == '\0')
		return (-1);
	word = ft_substr(input, start, *i - start);
	(*i)++;
	no_space = (input[*i] != '\0' && !is_blank(input[*i]));
	add_token(head, new_token(WORD, word, qt, no_space));
	free(word);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:43 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/08 21:50:42 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"

t_token	*new_token(t_token_type type, char *value, t_quote_type quote,
		int no_space)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->quote_type = quote;
	token->no_space = no_space;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*current;

	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}

static char	*scan_word(char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && input[*i] != ' ' && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

t_token	*lexer(char *input)
{
	t_token	*head;
	int		no_space;
	char	*word;
	int		i;

	head = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '\'')
			handle_single_quote(input, &i, &head);
		else if (input[i] == '"')
			handle_double_quote(input, &i, &head);
		else if (is_operator(input[i]))
			i += get_operator_token(input, &i, &head);
		else
		{
			word = scan_word(input, &i);
			no_space = (input[i] != ' ' && input[i] != '\0'
					&& !is_operator(input[i]));
			add_token(&head, new_token(WORD, word, NONE, no_space));
			free(word);
		}
	}
	return (head);
}

void	print_tokens(t_token *head)
{
	t_token	*current;

	current = head;
	while (current != NULL)
	{
		printf("[%d] \"%s\" (no_space: %d)\n", current->type, current->value,
				current->no_space);
		current = current->next;
	}
}

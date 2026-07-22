/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gbenetri          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"
#include "../../Includes/parser.h"

void	add_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*current_cmd;

	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current_cmd = *head;
	while (current_cmd->next != NULL)
		current_cmd = current_cmd->next;
	current_cmd->next = new;
}

static void	add_heredoc(t_cmd *cmd, char *delim, int expand)
{
	t_heredoc	*new;
	t_heredoc	*cur;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return ;
	new->delim = ft_strdup(delim);
	new->expand = expand;
	new->fd = -1;
	new->next = NULL;
	if (!cmd->heredocs)
		cmd->heredocs = new;
	else
	{
		cur = cmd->heredocs;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

void	handle_redir(t_token **token, t_cmd *cmd)
{
	t_token_type	type;

	type = (*token)->type;
	*token = (*token)->next;
	if (!*token)
		return ;
	if (type == HEREDOC)
		add_heredoc(cmd, (*token)->value, (*token)->quote_type == NONE);
	else
		add_redir(cmd, type, (*token)->value);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
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

void	handle_redir(t_token **token, t_cmd *cmd)
{
	t_token_type	type;

	type = (*token)->type;
	*token = (*token)->next;
	if (!*token)
		return ;
	if (type == HEREDOC)
	{
		free(cmd->heredoc);
		cmd->heredoc = ft_strdup((*token)->value);
	}
	else
		add_redir(cmd, type, (*token)->value);
}

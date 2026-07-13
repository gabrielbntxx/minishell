/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:52 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/18 00:40:15 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"
#include "../../Includes/parser.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->heredoc = NULL;
	cmd->hd_fd = -1;
	cmd->next = NULL;
	cmd->args_quote = NULL;
	return (cmd);
}

void	init_first_arg(t_cmd *cmd, char *arg)
{
	cmd->args = malloc(sizeof(char *) * 2);
	cmd->args[0] = ft_strdup(arg);
	cmd->args[1] = NULL;
}

void	add_arg(t_cmd *cmd, char *arg, t_quote_type quote)
{
	int		i;
	char	**new_args;
	int		*new_quotes;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	new_quotes = malloc(sizeof(int) * (i + 2));
	if (!new_quotes)
	{
		free(new_args);
		return ;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	new_quotes[i] = quote;
	new_quotes[i + 1] = 0;
	while (i-- > 0)
	{
		new_args[i] = cmd->args[i];
		new_quotes[i] = cmd->args_quote[i];
	}
	free(cmd->args);
	free(cmd->args_quote);
	cmd->args = new_args;
	cmd->args_quote = new_quotes;
}

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

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*cmd;

	head = NULL;
	cmd = new_cmd();
	add_cmd(&head, cmd);
	while (tokens != NULL)
	{
		if (((tokens)->type) == WORD)
			add_arg(cmd, tokens->value, tokens->quote_type);
		else if (((tokens)->type) == PIPE)
		{
			cmd = new_cmd();
			add_cmd(&head, cmd);
		}
		else
			handle_redir(&tokens, cmd);
		if (!tokens)
			break ;
		tokens = tokens->next;
	}
	return (head);
}

void	print_cmds(t_cmd *head)
{
	int		i;
	int		cmd_num;
	t_redir	*r;

	cmd_num = 1;
	while (head)
	{
		printf("=== CMD %d ===\n", cmd_num++);
		printf("args: ");
		i = 0;
		if (head->args)
		{
			while (head->args[i])
				printf("%s ", head->args[i++]);
		}
		printf("\n");
		r = head->redirs;
		while (r)
		{
			printf("redir type %d: %s\n", r->type, r->file);
			r = r->next;
		}
		printf("heredoc: %s\n", head->heredoc ? head->heredoc : "(null)");
		if (head->next)
			printf("--- PIPE ---\n");
		head = head->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:30 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"

void	rm_args(t_cmd *cmd)
{
	int	i;
	int	j;

	if (!cmd->args || !cmd->args[0])
		return ;
	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] != '\0' || cmd->args_quote[i] != NONE)
		{
			cmd->args[j] = cmd->args[i];
			cmd->args_quote[j] = cmd->args_quote[i];
			j++;
		}
		else
			free(cmd->args[i]);
		i++;
	}
	cmd->args[j] = NULL;
	cmd->args_quote[j] = 0;
}

static void	prepare_heredocs(t_cmd *cmd, t_shell *sh)
{
	while (cmd)
	{
		signal(SIGINT, handler1);
		handl_heredoc(cmd, sh);
		cmd = cmd->next;
		signal(SIGINT, handler0);
	}
}

static void	close_heredocs(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->hd_fd != -1)
			close(cmd->hd_fd);
		cmd->hd_fd = -1;
		cmd = cmd->next;
	}
}

int	super_exec(t_cmd *cmd, t_shell *sh)
{
	int	ret;

	if (!cmd)
		return (0);
	if (!cmd->args && !cmd->heredoc && !cmd->redirs)
		return (1);
	sh->head = cmd;
	prepare_heredocs(cmd, sh);
	if (cmd->next)
		ret = super_cmd(cmd, sh);
	else
		ret = base_cmd(cmd, sh);
	close_heredocs(cmd);
	if (ret == -2)
		return (-2);
	if (ret != 0)
		sh->status = ret;
	return (0);
}

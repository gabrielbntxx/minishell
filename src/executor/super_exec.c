/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:30 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/17 13:04:16 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>

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

static void	handler1(int sig)
{
	(void)sig;
	// printf("\n");
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// rl_redisplay();
	exit(0);
}

static void	heredoc_read(int fd, char *del)
{
	char	*str;

	signal(SIGINT, handler1);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		str = readline(">");
		if (!str)
			break ;
		if (!ft_strcmp(str, del))
		{
			free(str);
			break ;
		}
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
}

void	handl_heredoc(t_cmd *cmd)
{
	int	hd[2];
	int	pid;

	if (!cmd->heredoc)
		return ;
	if (pipe(hd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close(hd[0]);
		dup2(2, 1);
		heredoc_read(hd[1], cmd->heredoc);
		close(hd[1]);
		exit(0);
	}
	if (pid == -1)
	{
		close(hd[0]);
		close(hd[1]);
		return ;
	}
	waitpid(pid, NULL, 0);
	close(hd[1]);
	dup2(hd[0], STDIN_FILENO);
	close(hd[0]);
}

int	super_exec(t_cmd *cmd, t_env **env)
{
	int	ret;

	// char **array;
	if (!cmd)
		return (0);
	if (!cmd->args && !cmd->heredoc && !cmd->redirs)
		return (1);
	if (cmd->next)
		ret = super_cmd(cmd, env);
	else
		ret = base_cmd(cmd, env);
	if (ret == -2)
		return (-2);
	if (ret != 0)
		g_exit_st = ret;
	return (0);
}

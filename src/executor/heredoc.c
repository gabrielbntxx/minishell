/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"

static void	handler1(int sig)
{
	(void)sig;
	exit(0);
}

static void	write_line(int fd, char *str, int expand, t_env **env)
{
	if (expand)
		expand_one_arg(&str, env);
	if (str)
	{
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
}

static void	heredoc_read(int fd, char *del, int expand, t_env **env)
{
	char	*str;

	signal(SIGINT, handler1);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			str = readline(">");
		else
			str = read_line_notty();
		if (!str)
			break ;
		if (!ft_strcmp(str, del))
		{
			free(str);
			break ;
		}
		write_line(fd, str, expand, env);
	}
}

static void	heredoc_child(int hd[2], t_cmd *cmd, t_env **env)
{
	close(hd[0]);
	dup2(2, 1);
	heredoc_read(hd[1], cmd->heredoc, cmd->heredoc_expand, env);
	close(hd[1]);
	free_env(*env);
	exit(0);
}

void	handl_heredoc(t_cmd *cmd, t_env **env)
{
	int	hd[2];
	int	pid;

	if (!cmd->heredoc)
		return ;
	if (pipe(hd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
		heredoc_child(hd, cmd, env);
	if (pid == -1)
	{
		close(hd[0]);
		close(hd[1]);
		return ;
	}
	waitpid(pid, NULL, 0);
	close(hd[1]);
	cmd->hd_fd = hd[0];
}

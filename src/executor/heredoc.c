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

static void	heredoc_read(int fd, char *del)
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
	cmd->hd_fd = hd[0];
}

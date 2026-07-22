/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gbenetri          #+#    #+#             */
/*   Updated: 2026/07/21 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"

static void	write_line(int fd, char *str, int expand, t_shell *sh)
{
	if (expand)
		expand_one_arg(&str, sh);
	if (str)
	{
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
}

static void	heredoc_read(int fd, char *del, int expand, t_shell *sh)
{
	char	*str;

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
		write_line(fd, str, expand, sh);
	}
}

static void	heredoc_child(int hd[2], t_heredoc *node, t_shell *sh)
{
	close(hd[0]);
	dup2(2, 1);
	heredoc_read(hd[1], node->delim, node->expand, sh);
	close(hd[1]);
	exit_child(sh, 0, NULL);
}

static void	read_one_heredoc(t_heredoc *node, t_shell *sh)
{
	int	hd[2];
	int	pid;

	if (pipe(hd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, handler1);
		heredoc_child(hd, node, sh);
	}
	if (pid == -1)
	{
		close(hd[0]);
		close(hd[1]);
		return ;
	}
	waitpid(pid, NULL, 0);
	close(hd[1]);
	node->fd = hd[0];
}

void	handl_heredoc(t_cmd *cmd, t_shell *sh)
{
	t_heredoc	*node;

	node = cmd->heredocs;
	while (node)
	{
		if (g_signal)
			break ;
		read_one_heredoc(node, sh);
		if (node->next && node->fd != -1)
		{
			close(node->fd);
			node->fd = -1;
		}
		node = node->next;
	}
}

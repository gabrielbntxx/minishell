/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/21 14:05:18 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>

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

	while (1)
	{
		str = readline(">");
		if (!str) {
			break;
		}
		if (!ft_strcmp(str, del))
		{
			free(str);
			break ;
		}
		write_line(fd, str, expand, sh);
	}
}

static void	heredoc_child(int hd[2], t_cmd *cmd, t_shell *sh)
{
	dup2(2, 1);
	heredoc_read(hd[1], cmd->heredoc, cmd->heredoc_expand, sh);
	close(hd[1]);

}

void	handl_heredoc(t_cmd *cmd, t_shell *sh)
{
	int	hd[2];
	int stdin;


	if (!cmd->heredoc)
		return ;
	stdin = dup(0);
	if (pipe(hd) == -1)
		return ;

	heredoc_child(hd, cmd, sh);

	dup2(stdin, 0);
	cmd->hd_fd = hd[0];
	close(hd[0]);
}

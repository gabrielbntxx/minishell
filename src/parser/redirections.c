/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:52 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/11 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	add_redir(t_cmd *cmd, int type, char *file)
{
	t_redir	*new;
	t_redir	*cur;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->file = ft_strdup(file);
	new->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		cur = cmd->redirs;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

static int	redir_error(char *file)
{
	write(2, "minishell: ", 11);
	perror(file);
	return (1);
}

static int	open_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == APPEND)
		fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		return (redir_error(redir->file));
	if (redir->type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	take_heredoc_fd(t_cmd *cmd)
{
	t_heredoc	*node;
	int			fd;

	node = cmd->heredocs;
	if (!node)
		return (-1);
	while (node->next)
		node = node->next;
	fd = node->fd;
	node->fd = -1;
	return (fd);
}

int	apply_redir(t_cmd *cmd)
{
	t_redir	*redir;
	int		hd_fd;

	hd_fd = take_heredoc_fd(cmd);
	if (hd_fd != -1)
	{
		dup2(hd_fd, STDIN_FILENO);
		close(hd_fd);
	}
	redir = cmd->redirs;
	while (redir)
	{
		if (open_redir(redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}

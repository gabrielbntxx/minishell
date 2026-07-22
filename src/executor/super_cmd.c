/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

int	super_child(t_cmd *current, t_shell *sh, t_pipe_ctx *ctx)
{
	int	ret;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (ctx->last_fd != -1)
	{
		dup2(ctx->last_fd, STDIN_FILENO);
		close(ctx->last_fd);
	}
	if (current->next)
	{
		dup2(ctx->pipe_fd[1], STDOUT_FILENO);
		close(ctx->pipe_fd[0]);
		close(ctx->pipe_fd[1]);
	}
	if (apply_redir(current))
		exit_child(sh, 1, NULL);
	ret = dispatch(current, sh);
	if (ret == 1)
		execute_cmd(current, sh, 0, NULL);
	exit_child(sh, sh->status, NULL);
	return (0);
}

static int	fork_child(t_cmd *current, t_shell *sh, t_pipe_ctx *ctx)
{
	int	pid;

	signal(SIGINT, handler_parent);
	pid = fork();
	if (pid == 0)
		super_child(current, sh, ctx);
	signal(SIGINT, handler0);
	return (pid);
}

static void	link_pipe(t_cmd *current, t_pipe_ctx *ctx)
{
	if (current->next)
	{
		if (ctx->last_fd != -1)
			close(ctx->last_fd);
		close(ctx->pipe_fd[1]);
		ctx->last_fd = ctx->pipe_fd[0];
	}
	else if (ctx->last_fd != -1)
		close(ctx->last_fd);
}

int	super_cmd(t_cmd *cmd, t_shell *sh)
{
	t_pipe_ctx	ctx;
	t_cmd		*current;
	int			pid;
	int			status;

	ctx.last_fd = -1;
	current = cmd;
	while (current)
	{
		rm_args(current);
		if (current->next && pipe(ctx.pipe_fd) == -1)
			return (1);
		pid = fork_child(current, sh, &ctx);
		link_pipe(current, &ctx);
		current = current->next;
	}
	waitpid(pid, &status, 0);
	update_exit(status, sh);
	while (waitpid(-1, &status, 0) > 0)
		;
	return (sh->status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

int	super_child(t_cmd *current, t_env **env, t_pipe_ctx *ctx)
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
		exit_child(1, current, env);
	ret = dispatch(current, env);
	if (ret == 1)
		execute_cmd(current, env, 0);
	exit_child(g_exit_st, current, env);
	return (0);
}

static int	fork_child(t_cmd *current, t_env **env, t_pipe_ctx *ctx)
{
	int	pid;

	pid = fork();
	if (pid == 0)
		super_child(current, env, ctx);
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

int	super_cmd(t_cmd *cmd, t_env **env)
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
		pid = fork_child(current, env, &ctx);
		link_pipe(current, &ctx);
		current = current->next;
	}
	waitpid(pid, &status, 0);
	update_exit(status);
	while (wait(&status) > 0)
		;
	return (g_exit_st);
}

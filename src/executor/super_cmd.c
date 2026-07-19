/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

int	super_child(int last_fd, int *pipe_fd, t_shell *sh, t_cmd *current)
{
	int		ret;
	char	**array;

	array = env_to_array(*sh->env);
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
	if (current->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (apply_redir(current))
		exit_child(sh, 1, array);
	ret = dispatch(current, sh);
	if (ret == 1)
		execute_cmd(current, array, 0, sh);
	exit_child(sh, ret, array);
	return (0);
}

static int	fork_child(t_cmd *current, int last_fd, int *pipe_fd, t_shell *sh)
{
	int	pid;

	signal(SIGINT, handler1);
	pid = fork();
	if (pid == 0)
		super_child(last_fd, pipe_fd, sh, current);
	signal(SIGINT, handler0);
	return (pid);
}

static void	link_pipe(t_cmd *current, int *last_fd, int *pipe_fd)
{
	if (current->next)
	{
		if (*last_fd != -1)
			close(*last_fd);
		close(pipe_fd[1]);
		*last_fd = pipe_fd[0];
	}
	else if (*last_fd != -1)
		close(*last_fd);
}

int	super_cmd(t_cmd *cmd, t_shell *sh)
{
	int		pipe_fd[2];
	int		last_fd;
	int		pid;
	t_cmd	*current;

	last_fd = -1;
	current = cmd;
	while (current)
	{
		rm_args(current);
		if (current->next && pipe(pipe_fd) == -1)
			return (wait_all(sh, &last_fd), 1);
		pid = fork_child(current, last_fd, pipe_fd, sh);
		if (pid == -1)
			return (wait_all(sh, &last_fd), 1);
		save_pid(sh, pid);
		link_pipe(current, &last_fd, pipe_fd);
		current = current->next;
	}
	wait_all(sh, &last_fd);
	return (sh->status);
}

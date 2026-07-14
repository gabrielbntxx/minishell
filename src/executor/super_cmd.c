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

int	super_child(int last_fd, int *pipe_fd, t_env **env, t_cmd *current)
{
	int		ret;
	char	**array;

	array = env_to_array(*env);
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
		exit(1);
	ret = dispatch(current, env);
	if (ret == 1)
		execute_cmd(current, array, 0);
	exit(g_exit_st);
}

static int	fork_child(t_cmd *current, int last_fd, int *pipe_fd, t_env **env)
{
	int	pid;

	pid = fork();
	if (pid == 0)
		super_child(last_fd, pipe_fd, env, current);
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

int	super_cmd(t_cmd *cmd, t_env **env)
{
	int		pipe_fd[2];
	int		last_fd;
	int		pid;
	t_cmd	*current;
	int		status;

	last_fd = -1;
	current = cmd;
	while (current)
	{
		rm_args(current);
		if (current->next && pipe(pipe_fd) == -1)
			return (1);
		pid = fork_child(current, last_fd, pipe_fd, env);
		link_pipe(current, &last_fd, pipe_fd);
		current = current->next;
	}
	waitpid(pid, &status, 0);
	update_exit(status);
	while (wait(&status) > 0)
		;
	return (g_exit_st);
}

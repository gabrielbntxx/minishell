#include "../../Includes/executor.h"

void exit_child(t_shell *sh, t_cmd *cmd, int ret) {
    free_cmds(cmd);
    free_env(*sh->env);
    exit(ret);
}

int	super_child(int *pipe_fd, t_shell *sh, t_cmd *head, t_cmd *current)
{
	int		ret;
	char	**array;

	if (pipe_fd[4] != -1)
	{
		dup2(pipe_fd[4], STDIN_FILENO);
		close(pipe_fd[4]);
	}
	if (current->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (apply_redir(current))
		exit_child(sh, head, 1);
	ret = dispatch(current, sh);
	if (ret == 1)
	{
    	array = env_to_array(*sh->env);
			execute_cmd(current, array, 0, sh);
    	free_array(array);
  	}
	if (ret == -2)
		exit_child(sh, head, sh->status);
	if (ret == 0)
		exit_child(sh, head, 0);
	exit_child(sh, head, 127);
	return (0);
}

int	super_cmd(t_cmd *cmd, t_shell *sh)
{
	int		pipe_fd[4];
	int		pid;
	t_cmd	*current;
	int		status;

	pipe_fd[4] = -1;
	current = cmd;
	while (current)
	{
		rm_args(current);
		if (current->next)
			if (pipe(pipe_fd) == -1)
				return (1);
		pid = fork();
		if (pid == 0)
		{
			super_child(pipe_fd, sh, cmd, current);
		}
		if (current->next)
		{
			if (pipe_fd[4] != -1)
				close(pipe_fd[4]);
			close(pipe_fd[1]);
			pipe_fd[4] = pipe_fd[0];
		}
		else if (pipe_fd[4] != -1)
			close(pipe_fd[4]);
		current = current->next;
	}
	waitpid(pid, &status, 0);
	update_exit(status, sh);
	return (status);
}

void	ult_dup(int save[2], int mod)
{
	if (mod == 1)
	{
		dup2(save[0], STDIN_FILENO);
		dup2(save[1], STDOUT_FILENO);
		close(save[0]);
		close(save[1]);
		return ;
	}
	else if (mod == 2)
	{
		dup2(save[0], STDIN_FILENO);
		dup2(save[1], STDOUT_FILENO);
		close(save[0]);
		close(save[1]);
		return ;
	}
	else if (mod == 3)
	{
		dup2(save[0], STDIN_FILENO);
		dup2(save[1], STDOUT_FILENO);
		close(save[0]);
		close(save[1]);
		return ;
	}
}

int	base_cmd(t_cmd *cmd, t_shell *sh)
{
	int		save[2];
	int		ret;
	char	**array;

	rm_args(cmd);
	save[0] = dup(STDIN_FILENO);
	save[1] = dup(STDOUT_FILENO);
	if (apply_redir(cmd))
	{
		ult_dup(save, 3);
    sh->status = 1;
		return (1);
	}
	if (!cmd->args || !cmd->args[0])
	{
		ult_dup(save, 2);
		return (0);
	}
	ret = dispatch(cmd, sh);
	array = env_to_array(*sh->env);
	if (ret == 1)
		execute_cmd(cmd, array, 1, sh);
	ult_dup(save, 3);
	free_array(array);
	if (ret == -2)
		return (-2);
	return (0);
}

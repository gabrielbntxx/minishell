/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:27 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
}

void	exit_child(t_shell *sh, int code, char **array)
{
	if (array)
		free_array(array);
	free_cmds(sh->head);
	free_env(*sh->env);
	exit(code);
}

static void	exec_child(char *cmd_path, t_cmd *cmd, char **paths, t_shell *sh)
{
	char	**envp;
	int		code;

	envp = env_to_array(*sh->env);
	free_array(paths);
	execve(cmd_path, cmd->args, envp);
	write(2, "minishell: ", 11);
	perror(cmd->args[0]);
	code = 126;
	if (access(cmd_path, F_OK) != 0)
		code = 127;
	free(cmd_path);
	exit_child(sh, code, envp);
}

static int	spawn_child(int mod, int *save)
{
	int	pid;

	pid = -1;
	if (mod == 1)
		signal(SIGINT, handler1);
	if (mod == 1)
		pid = fork();
	if (pid == 0)
		signal(SIGQUIT, SIG_DFL);
	if (pid == 0 && save)
	{
		close(save[0]);
		close(save[1]);
	}
	return (pid);
}

void	execute_cmd(t_cmd *cmd, t_shell *sh, int mod, int *save)
{
	char	*cmd_path;
	char	**paths;
	int		pid;
	int		status;

	status = 0;
	cmd_path = resolve_cmd(cmd, sh, &paths);
	if (!cmd_path)
		return ;
	pid = spawn_child(mod, save);
	if (pid == 0 || mod == 0)
		exec_child(cmd_path, cmd, paths, sh);
	if (mod == 1)
		waitpid(pid, &status, 0);
	if (mod == 1)
		signal(SIGINT, handler0);
	free(cmd_path);
	free_array(paths);
	update_exit(status, sh);
}

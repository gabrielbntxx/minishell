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

void	exit_child(int code, t_cmd *cmd, t_env **env)
{
	free_cmds(cmd);
	free_env(*env);
	exit(code);
}

static void	exec_child(char *cmd_path, t_cmd *cmd, char **paths, t_env **env)
{
	char	**envp;
	int		code;

	envp = env_to_array(*env);
	free_array(paths);
	execve(cmd_path, cmd->args, envp);
	write(2, "minishell: ", 11);
	perror(cmd->args[0]);
	code = 126;
	if (access(cmd_path, F_OK) != 0)
		code = 127;
	free(cmd_path);
	free_array(envp);
	exit_child(code, cmd, env);
}

void	execute_cmd(t_cmd *cmd, t_env **env, int mod)
{
	char	**paths;
	char	*cmd_path;
	int		pid;
	int		status;

	pid = -1;
	status = 0;
	cmd_path = resolve_cmd(cmd, env, &paths);
	if (!cmd_path)
		return ;
	if (mod == 1)
		pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (pid == 0 || mod == 0)
		exec_child(cmd_path, cmd, paths, env);
	if (mod == 1)
		waitpid(pid, &status, 0);
	free(cmd_path);
	free_array(paths);
	update_exit(status);
}

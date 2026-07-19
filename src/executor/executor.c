/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:27 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
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

static void	exec_child(char *cmd_path, t_cmd *cmd, char **envp,
		t_shell *sh)
{
	execve(cmd_path, cmd->args, envp);
	free_array(envp);
	write(2, "minishell: ", 11);
	perror(cmd->args[0]);
	if (access(cmd_path, F_OK) != 0)
		sh->status = 127;
	else
		sh->status = 126;
	free(cmd_path);
	exit_child(sh, 0, NULL);
}

static char	*resolve_cmd(t_cmd *cmd, char **envp, char ***paths, t_shell *sh)
{
	char	*cmd_path;

	*paths = find_path(envp);
	if (!cmd->args || !cmd->args[0])
	{
		free_array(*paths);
		return (NULL);
	}
	cmd_path = find_cmd(*paths, cmd->args[0]);
	if (!cmd_path)
	{
		sh->status = 127;
		cmd_not_found(cmd->args);
		free_array(*paths);
	}
	return (cmd_path);
}

void	execute_cmd(t_cmd *cmd, char **envp, int mod, t_shell *sh)
{
	char	**paths;
	char	*cmd_path;
	int		pid;
	int		status;

	pid = -1;
	status = 0;
	cmd_path = resolve_cmd(cmd, envp, &paths, sh);
	if (!cmd_path)
		return ;
	sh->paths = paths;
	if (mod == 1)
		signal(SIGINT, handler1);
	if (mod == 1)
		pid = fork();
	if (pid == 0 || mod == 0)
		exec_child(cmd_path, cmd, envp, sh);
	if (mod == 1)
		waitpid(pid, &status, 0);
	if (mod == 1)
		signal(SIGINT, handler0);
	free(cmd_path);
	free_array(paths);
	sh->paths = NULL;
	update_exit(status, sh);
}

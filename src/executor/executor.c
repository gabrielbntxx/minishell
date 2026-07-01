/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:27 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/25 14:09:31 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Includes/executor.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i]) {
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
}


char	**find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}


char	*find_cmd(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char *tmp;

	i = 0;
    if (!cmd || ft_strlen(cmd) < 1)
		return (NULL);
	if (!*cmd || access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (paths && paths[i])
	{
		tmp = ft_strjoin(paths[i++], "/");
		path = ft_strjoin(tmp, cmd);
    free(tmp);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
	}
	return (NULL);
}


static void	cmd_not_found(char **args)
{
	if (args) {
		write(2, args[0], ft_strlen(args[0]));
	}
	write(2, ": command not found\n", 20);
	return;
}

void	execute_cmd(t_cmd *cmd, char **envp, int mod)
{
	char	**paths;
	char	*cmd_path;
	int		pid;
	int		status;

  pid = -1;
	paths = find_path(envp);
	if (!cmd->args || !cmd->args[0])
	{
		free_array(paths);
		return;
	}
	cmd_path = find_cmd(paths, cmd->args[0]);
	if (!cmd_path)
	{
		g_exit_st = 127;
		cmd_not_found(cmd->args);
		free_array(paths);
		return;
	}
  if (mod == 1)
	  pid = fork();
	if (pid == 0 || mod == 0)
	{
		free_array(paths);
		execve(cmd_path, cmd->args, envp);
		perror("minishell");
		free(cmd_path);
		exit(127);
	}
  if (mod == 1)
	  waitpid(pid, &status, 0);
	free(cmd_path);
	free_array(paths);
  update_exit(status);
	return;
}


// greob beug sur les commande de base genre ls 

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:27 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/10 17:22:12 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Includes/executor.h"
#include <sys/stat.h>

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
	if (!cmd || !*cmd || access(cmd, X_OK) == 0) {
		tmp = ft_strdup(cmd);
		return (tmp);
	}
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

static int	get_cmd_path(t_cmd *cmd, char **envp, char **out)
{
	char		**paths;
	struct stat	st;

	paths = find_path(envp);
	*out = find_cmd(paths, cmd->args[0]);
	free_array(paths);
	if (!*out)
		return (g_exit_st = 127, cmd_not_found(cmd->args), -1);
	if (stat(*out, &st) == 0 && S_ISDIR(st.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": Is a directory\n", 17);
		return (g_exit_st = 126, free(*out), -1);
	}
	return (0);
}

void	execute_cmd(t_cmd *cmd, char **envp)
{
	char	*cmd_path;
	int		pid;
	int		status;

	if (!cmd->args || !cmd->args[0])
		return ;
	if (get_cmd_path(cmd, envp, &cmd_path))
		return ;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, cmd->args, envp);
		perror("minishell");
		free(cmd_path);
		exit(127);
	}
	waitpid(pid, &status, 0);
	free(cmd_path);
	update_exit(status);
}


// greob beug sur les commande de base genre ls 

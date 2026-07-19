/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

void	save_pid(t_shell *sh, int pid)
{
	int		*new;
	int		i;

	new = malloc(sizeof(int) * (sh->pid_count + 1));
	if (!new)
		return ;
	i = 0;
	while (i < sh->pid_count)
	{
		new[i] = sh->pids[i];
		i++;
	}
	new[i] = pid;
	if (sh->pids)
		free(sh->pids);
	sh->pids = new;
	sh->pid_count++;
}

void	wait_all(t_shell *sh, int *last_fd)
{
	int	i;
	int	status;

	if (last_fd && *last_fd != -1)
		close(*last_fd);
	i = 0;
	while (i < sh->pid_count)
	{
		waitpid(sh->pids[i], &status, 0);
		i++;
	}
	if (sh->pid_count > 0)
		update_exit(status, sh);
	free(sh->pids);
	sh->pids = NULL;
	sh->pid_count = 0;
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
	char	*tmp;

	i = 0;
	if (!cmd || ft_strlen(cmd) < 1)
		return (NULL);
	if (ft_strchr(cmd, '/') != -1)
		return (ft_strdup(cmd));
	while (paths && paths[i])
	{
		tmp = ft_strjoin(paths[i++], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
	}
	return (NULL);
}

void	cmd_not_found(char **args)
{
	if (args)
	{
		write(2, args[0], ft_strlen(args[0]));
	}
	write(2, ": command not found\n", 20);
	return ;
}

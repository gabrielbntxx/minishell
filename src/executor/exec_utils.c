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
#include <sys/stat.h>

static int	is_dir(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

char	**find_path(t_env *env)
{
	char	*path;

	path = env_get(env, "PATH", 0);
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
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
		if (access(path, F_OK) == 0 && !is_dir(path))
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

char	*resolve_cmd(t_cmd *cmd, t_shell *sh, char ***paths)
{
	char	*cmd_path;

	*paths = find_path(*sh->env);
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

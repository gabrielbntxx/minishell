#include "../../Includes/executor.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		free(array[i++]);
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

	i = 0;
	if (!cmd || !*cmd || access(cmd, X_OK) == 0)
		return (cmd);
	while (paths && paths[i])
	{
		path = ft_strjoin(ft_strjoin(paths[i++], "/"), cmd);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
	}
	return (NULL);
}


static void	cmd_not_found(char **args, char **paths)
{
	if (args) {
		write(2, args[0], ft_strlen(args[0]));
		free_array(args);
		if (paths)
			free_array(paths);
	}
	write(2, ": command not found\n", 20);
	return;
}

void execute_cmd(t_cmd *cmd, char **envp) {
  char **paths;
  char *cmd_path;
	int pid;

  paths = find_path(envp);
  if (!cmd->args || !cmd->args[0]) {
    free_array(cmd->args);
    free_array(paths);
  }
	cmd_path = find_cmd(paths, cmd->args[0]);
	if (!cmd_path) {
		cmd_not_found(cmd->args, paths);
		return;
	}
	pid = fork();
	if (pid == 0)
		execve(cmd_path, cmd->args, envp);
	if (cmd_path)
			free(cmd_path);
	waitpid(pid, NULL, 0);
  free_array(cmd->args);
  free_array(paths);
		return;
}

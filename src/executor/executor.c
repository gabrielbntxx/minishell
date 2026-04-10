#include "executor.h"

void	free_array(char **a)
{
	int	i;

	i = 0;
	while (a && a[i])
		free(a[i++]);
	free(a);
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


char	*find_cmd(char **p, char *c)
{
	int		i;
	char	*path;

	i = 0;
	if (!c || !*c || access(c, X_OK) == 0)
		return (c);
	while (p && p[i])
	{
		path = ft_strjoin(p[i++], c);
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
	write(2, args[0], ft_strlen(args[0]));
	write(2, ": command not found\n", 20);
	free_array(args);
	free_array(paths);
	exit(127);
}

void execute_cmd(char *cmd, char **envp) {
  char **paths;
  char **args;
  char *cmd_path;

  paths = find_path(envp);
	args = ft_split(cmd, ' ');
  if (!args || !args[0]) {
    free_array(args);
    free_array(paths);
  }
	cmd_path = find_cmd(paths, cmd);
	if (!cmd_path)
		cmd_not_found(args, paths);
	execve(cmd_path, args, envp);
		if (cmd_path)
			free(cmd_path);
    free_array(args);
    free_array(paths);
    exit(126);
}

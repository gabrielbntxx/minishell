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


int second_exec(t_cmd *cmd, char *cmd_path, char **envp) {
  int pid;
  int fd_out;
  int fd_in;
  int flags;
	pid = fork();
	if (pid == 0) {
    if (cmd->redir_in) {
      fd_in = open(cmd->redir_in, O_RDONLY);
      if (fd_in == -1) {
        perror("can't open redir_in\n");
        exit(1);
      }
      dup2(fd_in, STDIN_FILENO);
      close(fd_in);
    }
    if (cmd->redir_out) {
      if (cmd->append)
        flags = O_WRONLY | O_CREAT | O_APPEND;
     else 
        flags = O_WRONLY | O_CREAT | O_TRUNC;
     fd_out = open(cmd->redir_out, flags, 0644);
     if (fd_out == -1) {
      perror("can't open redir_out\n");
      exit(1);
    }
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
  }
		execve(cmd_path, cmd->args, envp);
  }
	if (cmd_path)
			free(cmd_path);
  return(pid);
}

void execute_cmd(t_cmd *cmd, char **envp) {
  char **paths;
  char *cmd_path;

  paths = find_path(envp);
  if (!cmd->args || !cmd->args[0]) {
    free_array(cmd->args);
    free_array(paths);
    return;
  }
	cmd_path = find_cmd(paths, cmd->args[0]);
	if (!cmd_path) {
		cmd_not_found(cmd->args, paths);
		return;
	}

	waitpid(second_exec(cmd, cmd_path, envp), NULL, 0);
  free_array(cmd->args);
  free_array(paths);
		return;
}

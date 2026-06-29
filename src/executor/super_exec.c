/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:30 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/17 13:04:16 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>


static void rm_args(t_cmd *cmd) {
  int i;
  int j;

  if (!cmd->args || !cmd->args[0])
    return;
  i = 0;
  j = 0;
  while (cmd->args[i]) {
    if (cmd->args[i][0] != '\0' || cmd->args_quote[i] != NONE) {
      cmd->args[j] = cmd->args[i];
      cmd->args_quote[j] = cmd->args_quote[i];
      j++;
    }
    i++;
  }
  cmd->args[j] = NULL;
  cmd->args_quote[j] = 0;
}

  static void    handler1(int sig)
{
     (void) sig;
    // printf("\n");
    // rl_on_new_line();
    // rl_replace_line("", 0);
    // rl_redisplay();
    exit(0);
}

void handl_heredoc(t_cmd *cmd) {
  int hd[2];
  int pid;
  char *str;
  
  str = NULL;
  if (cmd->heredoc) {
    if(pipe(hd) == -1) return;
    pid = fork();
	if (pid == 0) {
		close(hd[0]);
		dup2(2, 1);
		while (1) {
        signal(SIGINT, handler1);
        signal(SIGQUIT, SIG_IGN); 

        str = readline(">");
        if (!str) break;
        if (!ft_strcmp(str, cmd->heredoc)) {
          free(str);
          break;
        }
        write(hd[1], str, ft_strlen(str));
        write(hd[1], "\n", 1);
        free(str);
      }
      close(hd[1]);
      exit(0);
    }
    else if (pid == -1) return;
    if (waitpid(pid, NULL, 0) == -1) return;
    close(hd[1]);
    if (dup2(hd[0], STDIN_FILENO) == -1) return;
    close(hd[0]);
  }
  return;
}




static int apply_redir(t_cmd *cmd) {
  int fd[2];
  
  handl_heredoc(cmd);
  if(cmd->redir_in) {
    fd[0] = open(cmd->redir_in, O_RDONLY);
    if (fd[0] == -1) return (1); 
    if (dup2(fd[0], STDIN_FILENO) == -1) return (1);
    

    close(fd[0]);
  } 
  if(cmd->redir_out) {
    if (cmd->append == 0)
      fd[1] = open(cmd->redir_out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else
      fd[1] = open(cmd->redir_out, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd[1] == -1) return (1);
    if (dup2(fd[1], STDOUT_FILENO) == -1) return (1);
    close(fd[1]);
  }
  return (0);
}

int super_cmd(t_cmd *cmd, char **array, t_env **env) {

  int pipe_fd[2];
  int last_fd = -1;
  int pid; 
  t_cmd *current;
  int status;
  int ret;

  current = cmd;
  while (current) {
      expand(current, env);
      rm_args(current);
      if (current->next) 
        if (pipe(pipe_fd) == -1) return (1);
      pid = fork();
    if (pid == 0) {
      if (last_fd != -1) { 
        dup2(last_fd, STDIN_FILENO);
        close(last_fd);
      } 
      if (current->next) { 
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
      }
      if (apply_redir(current))
        exit(1);
      ret = dispatch(current, env);
      if (ret == 1)
        execute_cmd(current, array, 0);
      if (ret == -2)
        exit(ret);
      if (ret == 0)
        exit(0);
      exit(127);
    }
    if (current->next) {
      if (last_fd != -1) close(last_fd);
      close(pipe_fd[1]);
      last_fd = pipe_fd[0];
    }
    else if (last_fd != -1) close(last_fd);

    current = current->next;
  }
  waitpid(pid, &status, 0);
  update_exit(status);
  return (status);
}

int base_cmd(t_cmd *cmd, char **array, t_env **env) {
  int save[2];
  int ret;

  expand(cmd, env);
  rm_args(cmd);
  save[0] = dup(STDIN_FILENO);
  save[1] = dup(STDOUT_FILENO);
  if (apply_redir(cmd)) {
    dup2(save[0], STDIN_FILENO);
    dup2(save[1], STDOUT_FILENO);
    close(save[0]);
    close(save[1]);
    g_exit_st = 1;
    return (1);
  }
  if (!cmd->args || !cmd->args[0]) {
    dup2(save[0], STDIN_FILENO);
    dup2(save[1], STDOUT_FILENO);
    close(save[0]);
    close(save[1]);
    return (0);
  }
  ret = dispatch(cmd, env);
  if (ret == 1)
    execute_cmd(cmd, array, 1);
  dup2(save[0], STDIN_FILENO);
  dup2(save[1], STDOUT_FILENO);
  close(save[0]);
  close(save[1]);
  if (ret == -2)
    return (-2);
  return (0);
}

int super_exec(t_cmd *cmd, t_env **env) {
  char **array; 
  int ret;

  if (!cmd) 
    return(0);
  array = env_to_array(*env);
    if (!cmd->args && !cmd->heredoc && !cmd->redir_in) {
      free_array(array);
      return(1);
    }
  if (cmd->next) 
    ret = super_cmd(cmd, array, env);
  else 
    ret = base_cmd(cmd, array, env);
  free_array(array);
  if (ret == -2)
    return(-2);
  return(0);
}

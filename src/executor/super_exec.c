/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:30 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/02 21:13:27 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>


char *find_deli(char *str, char *deli)
{
  int i;
  int j;

  if (!*deli | !str)
    return (str);
  i = 0;
  while (str[i])
  {
    j = 0;
    while (str[i + j] && str[i + j] == deli[j])
        j++;
    if (!deli[j])
        return (&str[i]);
    i++;
}
  return NULL;
  }


void handl_heredoc(t_cmd *cmd) {
  int hd[2];
  int pid;
  char *str;
  
  str = NULL;
  if (cmd->heredoc) {
    pipe(hd);
    pid = fork();
    if (pid == 0) {
      close(hd[0]);
      while (1) {
        str = readline(">");
        if (!ft_strcmp(str, cmd->heredoc)) break;
        write(hd[1], str, ft_strlen(str));
        write(hd[1], "\n", 1);
      }
      close(hd[1]);
      exit(0);
    }
    waitpid(pid, NULL, 0);
    close(hd[1]);
    dup2(hd[0], STDIN_FILENO);
    close(hd[0]);
  }
  return;
}




void apply_redir(t_cmd *cmd) {
  int fd[2];
  
  handl_heredoc(cmd);
  if(cmd->redir_in) {
    fd[0] = open(cmd->redir_in, O_RDONLY);
    dup2(fd[0], STDIN_FILENO);

    close(fd[0]);
  } 
  if(cmd->redir_out) {
    if (cmd->append == 0)
      fd[1] = open(cmd->redir_out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else
      fd[1] = open(cmd->redir_out, O_WRONLY | O_APPEND | O_CREAT, 0644);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
  }
  return;
}

void super_cmd(t_cmd *cmd, char **array, t_env *env) {

  int pipe_fd[2];
  int last_fd = -1;
  int pid; 
  t_cmd *current;

  current = cmd;
  while (current) {
      expand(cmd, env);

      if (current->next) pipe(pipe_fd);
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
      apply_redir(current);
      if (dispatch(current, &env) == 1)
        execute_cmd(current, array);
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
  while(wait(NULL) > 0);
}

void base_cmd(t_cmd *cmd, char **array, t_env *env) {
    expand(cmd, env);
  int save[2];
  save[0] = dup(STDIN_FILENO);
  save[1] = dup(STDOUT_FILENO);
  apply_redir(cmd);
  if (dispatch(cmd, &env) == 1)
  execute_cmd(cmd, array);
  dup2(save[0], STDIN_FILENO);
  dup2(save[1], STDOUT_FILENO);
  close(save[0]);
  close(save[1]);
}

void super_exec(t_cmd *cmd, t_env *env) {
  char **array; 
  array = env_to_array(env);

  if (!cmd->args) {
    if (cmd->heredoc) handl_heredoc(cmd);
    free_array(array);
    return;
  }
  if (cmd->next) super_cmd(cmd, array, env);
  else base_cmd(cmd, array, env);
  free_array(array);
}

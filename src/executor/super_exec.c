/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:30 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/02 22:03:21 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/env.h"
#include "../../Includes/executor.h"
#include "../../Includes/minishell.h"
#include <stdio.h>


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
      while (1) {
        signal(SIGINT, handler1);
        signal(SIGQUIT, SIG_IGN); 

        str = readline(">");
        if (!str) break;
        if (!ft_strcmp(str, cmd->heredoc)) break;
        write(hd[1], str, ft_strlen(str));
        write(hd[1], "\n", 1);
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




void apply_redir(t_cmd *cmd) {
  int fd[2];
  
  handl_heredoc(cmd);
  if(cmd->redir_in) {
    fd[0] = open(cmd->redir_in, O_RDONLY);
    if (fd[0] == -1) return; 
    if (dup2(fd[0], STDIN_FILENO) == -1) return;
    

    close(fd[0]);
  } 
  if(cmd->redir_out) {
    if (cmd->append == 0)
      fd[1] = open(cmd->redir_out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else
      fd[1] = open(cmd->redir_out, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd[1] == -1) return;
    if (dup2(fd[1], STDOUT_FILENO) == -1) return;
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
      expand(current, env);

      if (current->next) 
        if (pipe(pipe_fd) == -1) return;
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
      if (dispatch(current, env) == 1)
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

  if (!cmd) 
    return;
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

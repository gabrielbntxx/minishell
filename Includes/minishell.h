/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/readline.h>
# include "env.h"
# include "lexer.h"
# include "parser.h"

typedef struct s_shell
{
	t_env				**env;
	int					last_status;
	int					status;
	t_cmd				*head;
	int					*pids;
	int					pid_count;
	char				**paths;
}						t_shell;

# include "res.h"
# include "builtins.h"
# include "executor.h"
# include "expand.h"

extern int				g_signal;

void					handler0(int sig);
void					handler1(int sig);
void					update_exit(int status, t_shell *sh);
char					*read_line_notty(void);
void					free_cmds(t_cmd *cmd);
void					free_tokens(t_token *tok);
void					free_env(t_env *env);
void					free_all(t_token *tokens, t_cmd *cmds);
void					free_shell(t_shell *sh);
void					close_heredocs(t_cmd *cmd);
void					wait_all(t_shell *sh, int *last_fd);
void					register_pid(t_shell *sh, int pid);
int						is_redir(int type);
int						syntax_error(char *token, t_shell *sh);
int						token_value_error(t_token *token, t_shell *sh);
int						check_pipe(t_token *cur, t_shell *sh);
int						check_redir(t_token *cur, t_shell *sh);

#endif

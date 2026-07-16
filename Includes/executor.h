/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "parser.h"

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, unsigned int n);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(const char *s1, const char *s2);

void	free_array(char **a);
char	**find_path(t_env *env);
char	*find_cmd(char **p, char *c);
void	cmd_not_found(char **args);
char	*resolve_cmd(t_cmd *cmd, t_env **env, char ***paths);
void	exit_child(int code, t_cmd *cmd, t_env **env);
void	execute_cmd(t_cmd *cmd, t_env **env, int mod);

int		super_exec(t_cmd *cmd, t_env **env);
int		apply_redir(t_cmd *cmd);
void	handl_heredoc(t_cmd *cmd, t_env **env);
void	rm_args(t_cmd *cmd);
int		base_cmd(t_cmd *cmd, t_env **env);

typedef struct s_pipe_ctx
{
	int	last_fd;
	int	pipe_fd[2];
}	t_pipe_ctx;

int		super_cmd(t_cmd *cmd, t_env **env);
int		super_child(t_cmd *current, t_env **env, t_pipe_ctx *ctx);
void	ult_dup(int save[2], int mod);

#endif

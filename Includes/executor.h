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
char	**find_path(char **envp);
char	*find_cmd(char **p, char *c);
void	cmd_not_found(char **args);
void	execute_cmd(t_cmd *cmd, char **envp, int mod, t_shell *sh);

int		super_exec(t_cmd *cmd, t_shell *sh);
int		apply_redir(t_cmd *cmd);
void	handl_heredoc(t_cmd *cmd, t_shell *sh);
void	rm_args(t_cmd *cmd);
int		base_cmd(t_cmd *cmd, t_shell *sh);

int		super_cmd(t_cmd *cmd, t_shell *sh);
int		super_child(int last_fd, int *pipe_fd, t_shell *sh, t_cmd *current);
void	ult_dup(int save[2], int mod);
void exit_child(t_shell *sh, int ret, char **array);

#endif

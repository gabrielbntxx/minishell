#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"
#include "parser.h"

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, unsigned int n);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(const char *s1, const char *s2);

void	free_array(char **a);
char	**find_path(char **envp);
char	*find_cmd(char **p, char *c);
void	execute_cmd(t_cmd *cmd, char **envp, int mod);

int super_exec(t_cmd *cmd, t_env **env);
int apply_redir(t_cmd *cmd);
void handl_heredoc(t_cmd *cmd);
void rm_args(t_cmd *cmd);
int base_cmd(t_cmd *cmd, t_env **env);

int super_cmd(t_cmd *cmd, t_env **env);

#endif

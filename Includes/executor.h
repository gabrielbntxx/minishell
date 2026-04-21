#ifndef SPLIT_H
# define SPLIT_H


size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, unsigned int n);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(const char *s1, const char *s2);

void	free_array(char **a);
char	**find_path(char **envp);
char	*find_cmd(char **p, char *c);
void	execute_cmd(char *cmd, char **envp);

#endif

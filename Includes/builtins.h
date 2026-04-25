#ifndef BUILTINS_H
# define BUILTINS_H


char **sort_array(char **env);
char **env_to_export(t_env *env);
void print_export(char **env);
void ft_export(char **cmd, t_env **nodenv);

#endif

#ifndef BUILTINS_H
# define BUILTINS_H

#include "../../Includes/minishell.h"

char **sort_array(char **env);
char **env_to_export(t_env *env);
void print_export(char **env);
void ft_export(char **cmd, t_env **nodenv);
int main(int ac, char **av, char **envp);

#endif

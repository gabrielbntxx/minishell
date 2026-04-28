#include "../../Includes/minishell.h"



int dispatch(t_cmd *cmd, t_env *env) {
    char *this;
    this = cmd->args[0];

    if (!ft_strcmp(this, "export")) builtin_export(cmd->args, &env);
    else if (!ft_strcmp(this, "env")) builtin_env(cmd->args);
    else if (!ft_strcmp(this, "pwd")) builtin_pwd();
    else if (!ft_strcmp(this, "cd")) builtin_cd(cmd->args); 
    else if (!ft_strcmp(this, "echo")) builtin_echo(cmd->args);
    else if (!ft_strcmp(this, "exit")) builtin_exit(cmd->args, 0); //to-do = ajouter la gestion des valeur de retour 
    else if (!ft_strcmp(this, "unset")) builtin_unset(cmd->args, env);
    else return (1);
    return (0);
}
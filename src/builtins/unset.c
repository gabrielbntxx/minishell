#include "../../Includes/minishell.h"
#include "builtins.h"


void ft_unset(t_cmd *cmd, t_env *env) {
    int i = 1;

    while(cmd->args[i]) {
        env_unset(&env, cmd->args[i]);
        i++;
    }
    return;
}

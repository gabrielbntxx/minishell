#include "../../Includes/minishell.h"

void builtin_unset(char **args, t_env *env) {
    int i = 1;

    while(args[i]) {
        env_unset(&env, args[i]);
        i++;
    }
    return;
}

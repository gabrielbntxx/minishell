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

int main(int qc, char **av, char **envp) {
    t_env *env;
    t_cmd cmd;
    env = NULL;
    char *tet[] = {"export", NULL};

    tet[1] = NULL;
    init_env(envp, &env);
    env_set(&env, "test1", NULL);
    env_set(&env, "test2", NULL);
    env_set(&env, "test3", NULL);
    env_set(&env, "coucou", NULL);
    cmd.args = malloc(sizeof(char *) * 4);
    cmd.args[0] = "unset";
    cmd.args[1] = "test1";
    cmd.args[2] = "test2";
    cmd.args[3] = "test3";
    ft_export(tet,&env);
    ft_unset(&cmd, env);
    ft_export(tet ,&env);
}
#include "../../Includes/minishell.h"

void super_exec(t_cmd *cmd, t_env *env) {
    char = **array; 
    
    array = env_to_array(env);
    execute_cmd(cmd, array);
    
}
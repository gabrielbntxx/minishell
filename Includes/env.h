#ifndef ENV_H
# define ENV_H


typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

void    *env_get(t_env *env, char *key, int option);
void    env_set(t_env **env, char *key, char *value);
void    env_unset(t_env **env, char *key);
char    **env_to_array(t_env *env);
void    init_env(char **envp, t_env **first);

#endif

#ifndef BUILTINS_H
# define BUILTINS_H

/* builtins.c */
int dispatch(t_cmd *cmd, t_env *env);
/* cd.c */
int builtin_cd(char **args);
/* echo.c */
int is_n_flag(char *str);
int builtin_echo(char **args);
/* env.c */
int builtin_env(char **envp);
/* exit.c */
int ft_atoi(char *str);
int is_numeric(char *str);
int builtin_exit(char **args, int current_exit_status);
/* export.c */
char **sort_array(char **env);
char **env_to_export(t_env *env);
void print_export(char **env);
void builtin_export(char **cmd, t_env **nodenv);
/* pwd.c */
int builtin_pwd(void);
/* unset.c */
void builtin_unset(char **args, t_env *env);


#endif

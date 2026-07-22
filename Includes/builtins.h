/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/* builtins.c */
int		dispatch(t_cmd *cmd, t_shell *sh);
/* cd.c */
int		builtin_cd(t_env *env, char **args);
/* echo.c */
void	ft_putstr_fd(char *s, int fd);
int		is_n_flag(char *str);
int		builtin_echo(char **args);
/* env.c */
int		builtin_env(char **args, char **envp);
/* exit.c */
int		builtin_exit(char **args, t_shell *sh);
/* export.c */
char	**sort_array(char **env);
char	**env_to_export(t_env *env);
void	print_export(char **env);
int		is_valid(char *cmd);
int		export_append(char *arg, int sep, t_env **nodenv);
int		builtin_export(char **cmd, t_env **nodenv);
/* pwd.c */
int		builtin_pwd(void);
/* unset.c */
int		builtin_unset(char **args, t_env **env);

#endif

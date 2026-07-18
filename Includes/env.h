/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

void	*env_get(t_env *env, char *key, int option);
void	env_set(t_env **env, char *key, char *value);
void	env_unset(t_env **env, char *key);
char	**env_to_array(t_env *env);
void	init_env(char **envp, t_env **first);

typedef struct s_cmd	t_cmd;

typedef struct s_shell
{
	t_env	**env;
	int		last_status;
	int		status;
	t_cmd	*head;
}	t_shell;

#endif

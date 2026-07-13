/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:26:35 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/25 13:39:12 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	handle_builtin(t_cmd *cmd, t_env **env, char **envp)
{
	char	*this;
	int		ret;

	this = cmd->args[0];
	ret = 0;
	if (!ft_strcmp(this, "export"))
		ret = builtin_export(cmd->args, env);
	else if (!ft_strcmp(this, "env"))
		ret = builtin_env(cmd->args, envp);
	else if (!ft_strcmp(this, "pwd"))
		ret = builtin_pwd();
	else if (!ft_strcmp(this, "cd"))
		ret = builtin_cd(*env, cmd->args);
	else if (!ft_strcmp(this, "echo"))
		ret = builtin_echo(cmd->args);
	else if (!ft_strcmp(this, "unset"))
		ret = builtin_unset(cmd->args, env);
	else
		return (1);
	g_exit_st = ret;
	return (0);
}

int	dispatch(t_cmd *cmd, t_env **env)
{
	char	**envp;

	if (!cmd->args || !cmd->args[0])
		return (0);
	envp = env_to_array(*env);
	if (!ft_strcmp(cmd->args[0], "exit"))
	{
		free_array(envp);
		return (builtin_exit(cmd->args));
	}
	if (handle_builtin(cmd, env, envp))
	{
		free_array(envp);
		return (1);
	}
	free_array(envp);
	return (0);
}

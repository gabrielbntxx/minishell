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

static int	handle_builtin(t_cmd *cmd, t_shell *sh, char **envp)
{
	char	*this;
	int		ret;

	this = cmd->args[0];
	ret = 0;
	if (!ft_strcmp(this, "export"))
		sh->status = builtin_export(cmd->args, sh->env);
	else if (!ft_strcmp(this, "env"))
		sh->status = builtin_env(envp);
	else if (!ft_strcmp(this, "pwd"))
		sh->status = builtin_pwd();
	else if (!ft_strcmp(this, "cd"))
		sh->status = builtin_cd(*sh->env, cmd->args);
	else if (!ft_strcmp(this, "echo"))
		sh->status = builtin_echo(cmd->args);
	else if (!ft_strcmp(this, "unset"))
	{
		builtin_unset(cmd->args, sh->env);
		sh->status = 0;
	}
	else
		return (1);
	return (0);
}

int	dispatch(t_cmd *cmd, t_shell *sh)
{
	char	**envp;

	if (!cmd->args || !cmd->args[0])
		return (0);
	envp = env_to_array(*sh->env);
	if (!ft_strcmp(cmd->args[0], "exit"))
	{
		sh->status = builtin_exit(cmd->args, sh->last_status);
		free_array(envp);
		return (-2);
	}
	if (handle_builtin(cmd, sh, envp))
	{
		free_array(envp);
		return (1);
	}
	free_array(envp);
	return (0);
}

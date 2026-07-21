/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

void	ult_dup(int save[2], int mod)
{
	(void)mod;
	dup2(save[0], STDIN_FILENO);
	dup2(save[1], STDOUT_FILENO);
	close(save[0]);
	close(save[1]);
}

static int	finish_base(t_cmd *cmd, t_shell *sh, int save[2])
{
	int	ret;

	ret = dispatch(cmd, sh);
	if (ret == 1)
		execute_cmd(cmd, sh, 1, save);
	ult_dup(save, 3);
	return (ret);
}

int	base_cmd(t_cmd *cmd, t_shell *sh)
{
	int	save[2];
	int	ret;

	rm_args(cmd);
	save[0] = dup(STDIN_FILENO);
	save[1] = dup(STDOUT_FILENO);
	if (apply_redir(cmd))
	{
		ult_dup(save, 3);
		return (1);
	}
	if (!cmd->args || !cmd->args[0])
	{
		ult_dup(save, 2);
		sh->status = 0;
		return (0);
	}
	ret = finish_base(cmd, sh, save);
	if (ret == -2)
		return (-2);
	return (0);
}

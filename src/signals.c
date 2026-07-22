/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 00:00:00 by gabrielbene       #+#    #+#             */
/*   Updated: 2026/07/21 14:06:26 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	handler_parent(int sig)
{
	(void)sig;
	g_signal = 130;
	write(2, "\n", 1);
}

void	handler1(int sig)
{
	(void)sig;
	close(0);
}

void	handler0(int sig)
{
	(void)sig;
	g_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	update_exit(int status, t_shell *sh)
{
	if (WIFEXITED(status))
		sh->status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->status = 128 + WTERMSIG(status);
}

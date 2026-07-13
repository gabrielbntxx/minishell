/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:16 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/02 20:54:17 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"
#include "../../Includes/parser.h"

int	builtin_pwd(void)
{
	char	buff[1024];

	if (getcwd(buff, 1024) == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(buff, 1);
	write(1, "\n", 1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:20 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	builtin_unset(char **args, t_env **env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1])
		{
			write(2, "minishell: unset: `", 19);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': invalid option\n", 18);
			return (2);
		}
		env_unset(env, args[i]);
		i++;
	}
	return (0);
}

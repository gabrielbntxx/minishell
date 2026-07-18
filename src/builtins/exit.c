/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:26:55 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/25 13:21:45 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	parse_sign(char *str, int *i)
{
	int	sign;

	sign = 1;
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	parse_exit_arg(char *str, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	lim;
	unsigned long long	res;

	i = 0;
	sign = parse_sign(str, &i);
	lim = 9223372036854775807ULL + (unsigned long long)(sign == -1);
	if (!str[i])
		return (0);
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > (lim - (unsigned long long)(str[i] - '0')) / 10)
			return (0);
		res = res * 10 + (unsigned long long)(str[i++] - '0');
	}
	if (str[i])
		return (0);
	if (sign == -1)
		res = 0 - res;
	*out = (long long)res;
	return (1);
}

int	builtin_exit(char **args, t_shell *sh)
{
	long long	code;

	if (args[1] == NULL)
		return (-2);
	if (!parse_exit_arg(args[1], &code))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		sh->status = 2;
		return (-2);
	}
	if (args[2] != NULL)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		sh->status = 1;
		return (0);
	}
	sh->status = (unsigned char)code;
	return (-2);
}

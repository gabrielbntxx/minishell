/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:36:47 by mguilber          #+#    #+#             */
/*   Updated: 2025/11/03 19:22:56 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/res.h"

static size_t	ft_intlen(int nb)
{
	size_t	i;

	i = 0;
	if (nb < 0)
	{
		nb = nb * -1;
		i++;
	}
	if (nb >= 0 && nb <= 9)
		i++;
	else if (nb > 9)
	{
		while (nb / 10 > 0)
		{
			i++;
			nb = nb / 10;
		}
		i++;
	}
	return (i);
}

static char	ft_putnbr(int nb)
{
	char	c;

	c = nb + 48;
	return (c);
}

static char	*ft_rev_str(char *str, int size)
{
	int		i;
	char	temp;
	int		len;

	len = size;
	i = 0;
	temp = '\0';
	if (str[i] == '-')
	{
		i++;
		len++;
	}
	while (i < len / 2)
	{
		temp = str[i];
		str[i] = str[size - 1];
		str[size - 1] = temp;
		i++;
		size--;
	}
	return (str);
}

static char	*ft_int_to_char(int n, char *resu)
{
	size_t	i;

	i = 0;
	if (n < 0)
	{
		n = n * -1;
		resu[i] = '-';
		i++;
	}
	if (n >= 0 && n <= 9)
	{
		resu[i] = ft_putnbr(n);
		i++;
	}
	else
	{
		while (n > 0)
		{
			resu[i] = ft_putnbr(n % 10);
			n = n / 10;
			i++;
		}
	}
	ft_rev_str(resu, i);
	return (resu);
}

char	*ft_itoa(int n)
{
	char	*resu;
	char	*copy;
	size_t	i;

	i = 0;
	copy = "-2147483648";
	if (n == -2147483648)
	{
		resu = malloc(sizeof(char) * 12);
		if (!resu)
			return (0);
		while (copy[i])
		{
			resu[i] = copy[i];
			i++;
		}
		resu[i] = '\0';
		return (resu);
	}
	resu = malloc(sizeof(char) * ft_intlen(n) + 1);
	if (!resu)
		return (0);
	ft_int_to_char(n, resu);
	resu[ft_intlen(n)] = '\0';
	return (resu);
}

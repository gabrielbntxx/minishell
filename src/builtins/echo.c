/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:26:48 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/25 13:47:11 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"

int is_n_flag(char *str)
{
    int i;

    if (str[0] != '-')
        return(0);
    i = 1;
    while (str[i] != '\0')
    {
        if (str[i] != 'n')
            return(0);
        i++;
    }
    if (i == 1)
        return(0);
    return(1); 
}


int builtin_echo(char **args)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    while (args[i] != NULL && is_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    while (args[i] != NULL)
    {
        if (ft_strlen(args[i]) != 0) {
            printf("%s", args[i]);
            if (args[i + 1] != NULL && ft_strlen(args[i + 1]) != 0)
                printf(" ");
        }
        i++;
    }
    if (newline == 1)
        printf("\n");
    return(0);
}

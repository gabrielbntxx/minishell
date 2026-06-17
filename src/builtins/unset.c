/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:20 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/17 17:11:01 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void builtin_unset(char **args, t_env **env) {
    int i = 1;

    while(args[i]) {
        env_unset(env, args[i]);
        i++;
    }
    return;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenetri <gbenetri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

int		ft_strlcat(char *dst, const char *src, int size);
int		expand_one_arg(char **arg, t_shell *sh);
void	expand_tokens(t_token *tok, t_shell *sh);
void	field_split(t_token **head);
void	merge_tokens(t_token *tok);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   douane.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbentri <gbenetri@studen    				+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gbenetri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	is_redir(int type)
{
	if (type == REDIR_IN || type == REDIR_OUT || type == HEREDOC
		|| type == APPEND)
		return (1);
	return (0);
}

int	syntax_error(char *token, t_shell *sh)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	if (token)
		write(2, token, ft_strlen(token));
	else
		write(2, "newline", 7);
	write(2, "'\n", 2);
	sh->status = 2;
	return (1);
}

int	token_value_error(t_token *token, t_shell *sh)
{
	if (token)
		return (syntax_error(token->value, sh));
	return (syntax_error(NULL, sh));
}

int	check_pipe(t_token *cur, t_shell *sh)
{
	if (cur->type == PIPE && (!cur->next || cur->next->type == PIPE))
		return (token_value_error(cur->next, sh));
	return (0);
}

int	check_redir(t_token *cur, t_shell *sh)
{
	if (is_redir(cur->type) && (!cur->next || cur->next->type != WORD))
		return (token_value_error(cur->next, sh));
	return (0);
}

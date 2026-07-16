/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	int				*args_quote;
	t_redir			*redirs;
	char			*heredoc;
	int				heredoc_expand;
	int				hd_fd;
	struct s_cmd	*next;
}	t_cmd;

t_cmd		*new_cmd(void);
void		add_redir(t_cmd *cmd, int type, char *file);
t_cmd		*parser(t_token *tokens);
void		add_arg(t_cmd *cmd, char *arg, t_quote_type quote);
void		add_cmd(t_cmd **head, t_cmd *new);
void		init_first_arg(t_cmd *cmd, char *arg);
void		handle_redir(t_token **token, t_cmd *cmd);

#endif

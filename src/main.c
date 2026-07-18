/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:55:06 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/16 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/lexer.h"
#include "../Includes/minishell.h"
#include "../Includes/parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

int			g_signal = 0;

static int	validate_tokens(t_token *tokens, t_shell *sh)
{
	t_token	*cur;

	if (!tokens)
		return (0);
	if (tokens->type == PIPE)
		return (syntax_error(tokens->value, sh));
	cur = tokens;
	while (cur)
	{
		if (check_pipe(cur, sh))
			return (1);
		if (check_redir(cur, sh))
			return (1);
		cur = cur->next;
	}
	return (0);
}

static int	process_line(char *cmd, t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		ret;

	ret = 0;
	cmds = NULL;
	tokens = lexer(cmd);
	free(cmd);
	if (!validate_tokens(tokens, sh))
	{
		expand_tokens(tokens, sh);
		field_split(&tokens);
		merge_tokens(tokens);
		cmds = parser(tokens);
	}
	free_tokens(tokens);
	if (cmds)
		ret = super_exec(cmds, sh);
	free_cmds(cmds);
	return (ret);
}

static int	mini_loop(t_shell *sh)
{
	char	*cmd;
	int		ret;

	ret = 0;
	while (1)
	{
		signal(SIGINT, handler0);
		signal(SIGQUIT, SIG_IGN);
		sh->last_status = sh->status;
		g_signal = 0;
		if (isatty(STDIN_FILENO))
			cmd = readline("minishell> ");
		else
			cmd = read_line_notty();
		if (g_signal)
			sh->status = g_signal;
		if (!cmd)
			break ;
		if (*cmd)
			add_history(cmd);
		ret = process_line(cmd, sh);
		if (ret == -2)
			break ;
	}
	return (ret);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_shell	sh;

	(void)ac;
	(void)av;
	env = NULL;
	init_env(envp, &env);
	sh.status = 0;
	sh.last_status = 0;
	sh.env = &env;
	sh.head = NULL;
	mini_loop(&sh);
	free_env(env);
	return (sh.status);
}

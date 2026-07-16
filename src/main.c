/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:55:06 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/lexer.h"
#include "../Includes/minishell.h"
#include "../Includes/parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

int			g_exit_st = 0;

static int	validate_tokens(t_token *tokens)
{
	t_token	*cur;

	if (!tokens)
		return (0);
	if (tokens->type == PIPE)
		return (syntax_error(tokens->value));
	cur = tokens;
	while (cur)
	{
		if (check_pipe(cur))
			return (1);
		if (check_redir(cur))
			return (1);
		cur = cur->next;
	}
	return (0);
}

static int	process_line(char *cmd, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		ret;

	ret = 0;
	cmds = NULL;
	tokens = lexer(cmd);
	free(cmd);
	if (!validate_tokens(tokens))
	{
		expand_tokens(tokens, env);
		field_split(&tokens);
		merge_tokens(tokens);
		cmds = parser(tokens);
	}
	free_tokens(tokens);
	if (cmds)
		ret = super_exec(cmds, env);
	free_cmds(cmds);
	return (ret);
}

static int	mini_loop(t_env **env)
{
	char	*cmd;
	int		ret;

	ret = 0;
	while (1)
	{
		signal(SIGINT, handler0);
		signal(SIGQUIT, SIG_IGN);
		if (isatty(STDIN_FILENO))
			cmd = readline("minishell> ");
		else
			cmd = read_line_notty();
		if (!cmd)
			break ;
		if (*cmd)
			add_history(cmd);
		ret = process_line(cmd, env);
		if (ret == -2)
			break ;
	}
	return (ret);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;

	(void)ac;
	(void)av;
	env = NULL;
	init_env(envp, &env);
	mini_loop(&env);
	free_env(env);
	return (g_exit_st);
}

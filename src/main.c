/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:55:06 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/08 21:57:00 by gabrielbene      ###   ########.fr       */
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

void	handler0(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	update_exit(int status)
{
	if (WIFEXITED(status))
		g_exit_st = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_st = 128 + WTERMSIG(status);
}

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

static int	mini_loop(t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	char	*cmd;
	int		ret;

	ret = 0;
	while (1)
	{
		signal(SIGINT, handler0);
		signal(SIGQUIT, SIG_IGN);
		cmds = NULL;
		tokens = NULL;
		cmd = readline("minishell> ");
		if (!cmd)
			break ;
		if (*cmd)
			add_history(cmd);
		tokens = lexer(cmd);
		if (!validate_tokens(tokens))
		{
			expand_tokens(tokens, env);
			merge_tokens(tokens);
			cmds = parser(tokens);
			if (cmds)
				ret = super_exec(cmds, env);
		}
		free(cmd);
		free_all(tokens, cmds);
		if (ret == -2)
			break ;
	}
	return (ret);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	int		ret;

	(void)ac;
	(void)av;
	env = NULL;
	init_env(envp, &env);
	ret = mini_loop(&env);
	free_env(env);
	return (g_exit_st);
}

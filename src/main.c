/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:55:06 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/02 21:11:55 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"
#include "../Includes/lexer.h"
#include "../Includes/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

static void    handler0(int sig)
{
    (void) sig;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

#include <stdlib.h>
#include <string.h>

void free_tokens(t_token *tok)
{
    t_token *tmp;
    while (tok)
    {
        tmp = tok->next;
        free(tok->value);
        free(tok);
        tok = tmp;
    }
}

void free_cmds(t_cmd *cmd)
{
    t_cmd *tmp;
    while (cmd)
    {
        tmp = cmd->next;
        free_array(cmd->args);
        free(cmd->redir_in);
        free(cmd->redir_out);
        free(cmd->heredoc);
        free(cmd);
        cmd = tmp;
    }
}

void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void free_all(t_token *tokens, t_cmd *cmds)
{
    free_tokens(tokens);
    free_cmds(cmds);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_token	*tokens;
	char	*cmd;
  t_cmd *cmds;
	(void)ac;
	(void)av;
	env = NULL;
	init_env(envp, &env);
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
    cmds = parser(tokens);
		super_exec(cmds, env);
		free(cmd);
	}
	free_all(tokens, cmds);
	free_env(env);
	return (0);
}

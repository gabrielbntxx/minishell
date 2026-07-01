/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguilber <mguilber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:55:06 by mguilber          #+#    #+#             */
/*   Updated: 2026/06/11 13:02:36 by mguilber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"
#include "../Includes/lexer.h"
#include "../Includes/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

void    handler0(int sig)
{
    (void) sig;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

#include <stdlib.h>
#include <string.h>

int g_exit_st = 0;

void update_exit(int status) {
  if (WIFEXITED(status))
      g_exit_st = WEXITSTATUS(status);
  else if (WIFSIGNALED(status))
      g_exit_st = 128 + WTERMSIG(status);
}

void free_tokens(t_token *tok)
{
    t_token *tmp;
    while (tok)
    {
        //if (tmp)
            tmp = tok->next;
        if (tok->value)
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
        if (cmd->args)
            free_array(cmd->args);
        if (cmd->redir_in)
            free(cmd->redir_in);
        if (cmd->redir_out)
            free(cmd->redir_out);
        if (cmd->heredoc)
            free(cmd->heredoc);
        if (cmd->args_quote)
            free(cmd->args_quote);
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
    if (tokens)
      free_tokens(tokens);
    if (cmds)
        free_cmds(cmds);

}

static int	is_redir(int type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == HEREDOC || type == APPEND)
		return (1);
	return (0);
}

static int  syntax_error(char *token)
{
    write(2, "minishell: syntax error near unexpected token `", 47);
    if (token)
        write(2, token, ft_strlen(token));
    else
        write(2, "newline", 7);
    write(2, "'\n", 2);
    g_exit_st = 2;
    return (1);
}

static int	token_value_error(t_token *token)
{
	if (token)
		return (syntax_error(token->value));
	return (syntax_error(NULL));
}

static int	check_pipe(t_token *cur)
{
	if (cur->type == PIPE && (!cur->next || cur->next->type == PIPE))
		return (token_value_error(cur->next));
	return (0);
}

static int	check_redir(t_token *cur)
{
	if (is_redir(cur->type)
		&& (!cur->next || cur->next->type != WORD))
		return (token_value_error(cur->next));
	return (0);
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

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_token	*tokens;
	char	*cmd;
  int ret;
  t_cmd *cmds;
  
  ret = 0;
  tokens = NULL;
  cmds = NULL;
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
    if (!validate_tokens(tokens)) {
      cmds = parser(tokens);
      if (cmds)
		    ret = super_exec(cmds, &env);
    }
		free(cmd);
	  free_all(tokens, cmds);
    if (ret == -2) break;
	}
	free_env(env);
	return (g_exit_st);
}

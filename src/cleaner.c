#include "../Includes/minishell.h"

void	free_tokens(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok->next;
		if (tok->value)
			free(tok->value);
		free(tok);
		tok = tmp;
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

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
    //if (cmd->head)
      //free(cmd->head);
    if(cmd)
		  free(cmd);
		cmd = tmp;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_all(t_token *tokens, t_cmd *cmds)
{
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		free_cmds(cmds);
}

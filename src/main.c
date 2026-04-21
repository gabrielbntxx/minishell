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

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_token	*tokens;
	char	*cmd;
	char	**array;
  t_cmd *cmds;
	(void)ac;
	(void)av;
	env = NULL;
	init_env(envp, &env);
	array = env_to_array(env);
	while (1)
	{
    signal(SIGINT, handler0);
    signal(SIGQUIT, SIG_IGN); 
		cmd = readline("minishell> ");
		if (!cmd)
			break ;
		if (*cmd)
			add_history(cmd);
		tokens = lexer(cmd);
    cmds = parser(tokens);
    printf("---- LEXER ----\n");
		print_tokens(tokens);
		printf("---- command ----\n");
    print_cmds(cmds); 
    printf("---- EXEC ----\n");
		execute_cmd(cmds, array);
		free(cmd);
	}
	return (0);
}

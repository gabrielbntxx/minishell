#include "../Includes/minishell.h"
#include "../Includes/lexer.h"
#include "../Includes/parser.h"
#include <readline/readline.h>
#include <readline/history.h>


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

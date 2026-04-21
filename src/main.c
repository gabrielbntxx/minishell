#include "../Includes/minishell.h"
#include "../Includes/lexer.h"
#include "../Includes/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = lexer(input);
		printf("--- TOKENS ---\n");
		print_tokens(tokens);
		cmds = parser(tokens);
		printf("--- CMDS ---\n");
		print_cmds(cmds);
		free(input);
	}
	return (0);
}

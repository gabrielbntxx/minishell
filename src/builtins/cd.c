#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"

int builtin_cd(char **args)
{
    char *path;

    if (args[1] == NULL)
    {
        path = getenv("HOME");
        if (path == NULL)
        {
            write(2, "minishell: cd: HOME not set\n", 28);
            return(1);
        } 
    }
    else
    {
        path = args[1];
    }
    if (chdir(path) == -1)
    {
        perror("minishell: cd");
        return(1);
    }
    return(0);
}

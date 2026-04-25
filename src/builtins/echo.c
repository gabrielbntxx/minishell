#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"

int is_n_flag(char *str)
{
    int i;

    if (str[0] != '-')
        return(0);
    i = 1;
    while (str[i] != '\0')
    {
        if (str[i] != 'n')
            return(0);
        i++;
    }
    if (i == 1)
        return(0);
    return(1); 
}


int builtin_echo(char **args)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    while (args[i] != NULL && is_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    while (args[i] != NULL)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
            printf(" ");
        i++;
    }
    if (newline == 1)
        printf("\n");
    return(0);
}

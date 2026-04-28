#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"


int builtin_pwd(void)
{
    char buff[1024];
    if (getcwd(buff, 1024) == NULL)
    {
        perror("pwd");
        return(1);
    }
    printf("%s\n", buff);
    return(0);
}

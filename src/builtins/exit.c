
#include "../../Includes/minishell.h"


int ft_atoi(char *str)
{
    int i = 0;
    int sign = 1;
    int result = 0;

    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
	|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
	{
		i++;
	}
    if(str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
        {
            sign = -1;
        }
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        result = (result * 10) + str[i] - '0';
        i++;
    }
    return(result * sign);
}



int builtin_exit(char **args, int current_exit_status)
{
    printf("exit\n");
    if (args[1] == NULL)
        exit(current_exit_status);
    if (!is_numeric(args[1]))
    {
        printf("minishell: exit : %s: numeric argument required\n", args[1]);
        exit(2);
    }
    if (args[2] != NULL)
    {
        printf("minishell: exit: too many arguments\n");
        return(1);
    }
    exit(ft_atoi(args[1]) % 256);
}
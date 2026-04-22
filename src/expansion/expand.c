#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"


char    *get_var_value(char *name, int exit_status)
{
    char *value;

    if (name[0] == '?' && name[1] == '\0')
        return(ft_itoa(exit_status));
    else
    {
        value = getenv(name);
        if (value == NULL)
            return(NULL);
        return(value);
    }
}    


char    *get_var_name(char *str, int *i)
{
    int start;
    if (str[*i] == '?')
    {
        (*i)++;
        return("?"); 
    }
    start = *i;
    while (str[*i] >= 'a' && str[*i] <= 'z' || str[*i] >= 'A' && str[*i] <= 'Z' || str[*i] >= '0' && str[*i] <= '9' || str[*i] == '_')
        (*i)++;
    return(ft_substr(str, start, *i - start));
}

char    *append_char(char *str, char c)
{
    char *result;
    int i;
    i = 0;
    if (str == NULL)
    {
        result = malloc(2);
        result[0] = c;
        result[1] = '\0';
        return(result);
    }
    while (str[i] != '\0')
        i++;
    result = malloc(i + 2);
    ft_strlcpy(result, str, i + 1);
    result[i] = c;
    result[i + 1] = '\0';
    free(str);
    return(result);  
}    


char    *append_str(char *str, char *to_add) 
{
    char *result;
    if (str == NULL)
        return(ft_strdup(to_add));
    if (to_add == NULL)
        return(str);
    result = ft_strjoin(str, to_add);
    free(str);
    return(result);
}                                                                                                    

char    *expand_str(char *str, int exit_status)
{
    char    *result;
    char    *name;                                                                                                                                         
    char    *value;
    int i;
    result = NULL;
    i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '$')
        {
            i++;
            name = get_var_name(str, &i);
            value = get_var_value(name, exit_status);
            result = append_str(result, value);
        }
        else
        {
            result = append_char(result, str[i]);
            i++;
        }
    }
    return(result);
}


#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"

int    is_operator(char c)
{
    if (c != '<' && c != '>' && c != '|' )
    {
        return(0);
    }
    return(1);
}

int    get_operator_token(char *input, int *i, t_token **head)
{
    if (input[*i] == '|')
    {
        add_token(head, new_token(PIPE, "|", NONE, 0));
        return(1);
    }
    else if (input[*i] == '>' && input[*i + 1] == '>')
    {
        add_token(head, new_token(APPEND, ">>", NONE, 0));  
        return(2);
    }
    else if (input[*i] == '<' && input[*i + 1] == '<')
    {
        add_token(head, new_token(HEREDOC, "<<", NONE, 0)); 
        return(2);
    }
    else if (input[*i] == '>')
    {
        add_token(head, new_token(REDIR_OUT, ">", NONE, 0));
        return(1);
    }
    else if (input[*i] == '<')
    {
        add_token(head, new_token(REDIR_IN, "<", NONE, 0));
        return(1);
    }  
    return(0);
}

int    handle_single_quote(char *input, int *i, t_token **head)
{
    int start;
    int no_space;
    char *word;
    (*i)++;
    start = *i;
    while(input[*i] && input[*i] != '\'')
        (*i)++;
    if (input[*i] == '\0')
        return(-1);
    word = ft_substr(input, start, *i - start);   // 5. Extraire  
    (*i)++;    
    no_space = (input[*i] != ' ' && input[*i] != '\0');            
    add_token(head, new_token(WORD, word, SINGLE, no_space));  // 6. Ajouter    
    free(word);
    return(0);
} 

int    handle_double_quote(char *input, int *i, t_token **head)
{
    int start;
    int no_space;
    char *word;
    (*i)++;
    start = *i;
    while(input[*i] && input[*i] != '"')
        (*i)++;
    if (input[*i] == '\0')
        return(-1);
    word = ft_substr(input, start, *i - start);   // 5. Extraire
    (*i)++;    
    no_space = (input[*i] != ' ' && input[*i] != '\0');    
    add_token(head, new_token(WORD, word, DOUBLE, no_space));  // 6. Ajouter    
    free(word);
    return(0);
}


 
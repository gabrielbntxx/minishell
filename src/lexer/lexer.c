#include "lexer.h"
#include "minishell.h"

t_token    *new_token(t_token_type type, char *value, t_quote_type quote, int no_space)
{
    t_token *token;
    token = malloc(sizeof(t_token));
    if (token == NULL)
        return(NULL);
    token->type = type;
    token->value = ft_strdup(value);
    token->quote_type = quote;
    token->no_space = no_space;
    token->next = NULL;
    return(token);
}

void  add_token(t_token **head, t_token *new)
{
    t_token *current;
    if (*head == NULL)
    {
        *head = new;
        return;
    }
    current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new;  
}                             

t_token *lexer(char *input)
{
    t_token head;
    int start;
    int i;
    i = 0;

    while (input[i] == ' ')
        i++;

    if (input[i] == '\0')
    {
        break;
    }
    char *word;
    start = i;
    while (input[i] != '\0' && input[i] != ' ')
        i++;
    while (i > start)
    {
        word = substring(input, start, i);                                                                                                                                                                                      
    }
    add_token(&head, new_token(WORD, word, NONE, 0));
    free(word);
}
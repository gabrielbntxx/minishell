#include "../../Includes/lexer.h"
#include "../../Includes/minishell.h"

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
    t_token *head = NULL;
    int start;
    int no_space;
    char *word;
    int i;
    i = 0;

    while (input[i] != '\0')
    {
        while (input[i] == ' ')
            i++;
        
        if (input[i] == '\0')
            break;
        if (input[i] == '\'')
            handle_single_quote(input, &i, &head);
        else if (input[i] == '"')                                                                           
            handle_double_quote(input, &i, &head);
        else if (is_operator(input[i]))
            i += get_operator_token(input, &i, &head);
        else
        { 
            start = i;
            while (input[i] != '\0' && input[i] != ' ' && !is_operator(input[i]))
                i++;
            no_space = (input[i] != ' ' && input[i] != '\0' && !is_operator(input[i]));
            word = ft_substr(input, start, i - start);   // 5. Extraire            
            add_token(&head, new_token(WORD, word, NONE, no_space));  // 6. Ajouter       
            free(word);  
        }           
    }
    return(head);
}

 void  print_tokens(t_token *head)
 {
    t_token *current;
    current = head;
    while (current != NULL)
    {
        printf("[%d] \"%s\" (no_space: %d)\n", current->type, current->value, current->no_space);
        current = current->next;
    }
 }

 int main(void)
 {
    t_token *tokens;
    tokens = lexer("echo \"hello\"'world' test");                                                                                                
    print_tokens(tokens);
    return(0);
 }

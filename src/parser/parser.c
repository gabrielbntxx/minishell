#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"

t_cmd   *new_cmd(void)
{
    t_cmd *cmd;
    cmd = malloc(sizeof(t_cmd));
    if(cmd == NULL)
        return(NULL);
    cmd->args = NULL;
    cmd->redir_in = NULL;
    cmd->redir_out = NULL;
    cmd->append = 0;
    cmd->heredoc = NULL;
    cmd->next = NULL;
    return(cmd);
}

void    init_first_arg(t_cmd *cmd, char *arg)
{
     cmd->args = malloc(sizeof(char *) * 2);
     cmd->args[0] = ft_strdup(arg);
     cmd->args[1] = NULL;
}

void    add_arg(t_cmd *cmd, char *arg)
{
    int count;
    int i;
    char **new_args;
    count = 0;
    i = 0;
    if (cmd->args == NULL)
       init_first_arg(cmd, arg);                                                                          
    else
    {
        while (cmd->args[count] != NULL)
            count++;
        new_args = malloc(sizeof(char *) * (count + 2));
        while (cmd->args[i] != NULL)
        {
            new_args[i] = cmd->args[i];
            i++;
        }
        new_args[i] = ft_strdup(arg);
        new_args[i + 1] = NULL;
        free(cmd->args);
        cmd->args = new_args;   
    }
}       


void    add_cmd(t_cmd **head, t_cmd *new)
{
    t_cmd *current_cmd;
    if (*head == NULL)
    {
        *head = new;
        return;
    }
    current_cmd = *head;
    while (current_cmd->next != NULL)
        current_cmd = current_cmd->next;
    current_cmd->next = new;
}   


void    handle_redir(t_token **token, t_cmd *cmd)
{
    if (((*token)->type) == REDIR_IN)
    {
        *token = (*token)->next;
        cmd->redir_in = (*token)->value;
    }
    else if (((*token)->type) == REDIR_OUT)
    {
        *token = (*token)->next;
        cmd->redir_out = (*token)->value, cmd->append = 0;
    }
    else if (((*token)->type) == APPEND)
    {
        *token = (*token)->next;
        cmd->redir_out = (*token)->value, cmd->append = 1;
    }
    else if (((*token)->type) == HEREDOC)
    {
        *token = (*token)->next;
        cmd->heredoc = (*token)->value;
    }
}                                                                          


t_cmd   *parser(t_token *tokens)
{
    t_cmd *head;
    t_cmd *cmd;
    head = NULL;
    cmd = new_cmd();
    add_cmd(&head, cmd);

    while (tokens != NULL)
    {
        if (((tokens)->type) == WORD)
            add_arg(cmd, tokens->value);
        else if (((tokens)->type) == PIPE)
        {
            cmd = new_cmd();
            add_cmd(&head, cmd);
        }
        else
            handle_redir(&tokens, cmd);
        tokens = tokens->next;
    }
    return(head);
}                                                                                        


void    print_cmds(t_cmd *head)                                                                                              
{                                                                                                                            
    int i;                                                                                                                   
    int cmd_num;                                                                                                             
                                                                                                                               
    cmd_num = 1;                                                                                                             
    while (head)                                                                                                             
    {                                                                                                                        
        printf("=== CMD %d ===\n", cmd_num++);                                                                               
        printf("args: ");                                                                                                    
        i = 0;                                                                                                               
        if (head->args)                                                                                                      
        {                                                                                                                    
            while (head->args[i])                                                                                            
                printf("%s ", head->args[i++]);                                                                              
        }                                                                                                                    
        printf("\n");                                                                                                        
        printf("redir_in: %s\n", head->redir_in ? head->redir_in : "(null)");                                                
        printf("redir_out: %s\n", head->redir_out ? head->redir_out : "(null)");                                             
        printf("append: %d\n", head->append);                                                                                
        printf("heredoc: %s\n", head->heredoc ? head->heredoc : "(null)");                                                   
        if (head->next)                                                                                                      
            printf("--- PIPE ---\n");                                                                                        
        head = head->next;                                                                                                   
    }                                                                                                                        
}                                                                                                                            
                                                                                                  
int main(void)                                                                                                               
{                                                                                                                            
    t_token *tokens;                                                                                                         
    t_cmd *cmds;                                                                                                             
                                                                                                                               
    tokens = lexer("cat < in.txt >> out.txt");                                                                                  
    cmds = parser(tokens);                                                                                                   
    print_cmds(cmds);                                                                                                        
    return (0);                                                                                                              
}               



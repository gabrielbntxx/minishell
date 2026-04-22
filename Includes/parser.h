#ifndef PARSER_H
#define PARSER_H


#include "lexer.h" 

typedef struct s_cmd
{
    char    **args;  // ["cmd", "arg1", "arg2", NULL]
    char    *redir_in;   // fichier pour < 
    char    *redir_out;    // fichier pour > ou >>                                                    
    int     append;      // 1 si >>, 0 si >
    char    *heredoc;     // délimiteur pour <<  
    struct s_cmd    *next;   // commande suivante (après |)
}   t_cmd;


t_cmd   *new_cmd(void);
t_cmd   *parser(t_token *tokens); 
void    add_arg(t_cmd *cmd, char *arg);         
void    add_cmd(t_cmd **head, t_cmd *new);                                                                                   
void    print_cmds(t_cmd *head);      
void    init_first_arg(t_cmd *cmd, char *arg);  
                                                                                     


#endif
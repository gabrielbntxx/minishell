#ifndef PARSER_H
#define PARSER_H


#include "lexer.h" 

typedef struct s_redir
{
    int             type;   // REDIR_IN, REDIR_OUT ou APPEND
    char            *file;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char    **args;  // ["cmd", "arg1", "arg2", NULL]
    int     *args_quote;
    t_redir *redirs;      // toutes les redirections < > >>, dans l'ordre
    char    *heredoc;     // délimiteur pour <<
    struct s_cmd    *next;   // commande suivante (après |)
}   t_cmd;


t_cmd   *new_cmd(void);
void    add_redir(t_cmd *cmd, int type, char *file);
t_cmd   *parser(t_token *tokens); 
void    add_arg(t_cmd *cmd, char *arg, t_quote_type quote);   
void    add_cmd(t_cmd **head, t_cmd *new);                                                                                   
void    print_cmds(t_cmd *head);      
void    init_first_arg(t_cmd *cmd, char *arg);                                                                                       


#endif

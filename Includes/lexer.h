#ifndef LEXER_H
#define LEXER_H

typedef enum e_token_type                                                      
{                                                                              
    WORD,                                                                      
    PIPE,                                                                      
    REDIR_IN,                                                                  
    REDIR_OUT,                                                                 
    HEREDOC,                                                                   
    APPEND                                                                     
}   t_token_type;

typedef enum e_quote_type
{
    NONE,
    SINGLE,
    DOUBLE
}   t_quote_type;


typedef struct s_token                                                         
{                                                                              
    t_token_type    type;      // le enum                                      
    char            *value;                                                    
    t_quote_type    quote_type;                                                
    int             no_space;                                                  
    struct s_token  *next;                                                     
}   t_token; 

#endif
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


char	*ft_substr(char const *s, unsigned int start, int len);
int	    ft_strlcpy(char *dst, const char *src, int dstsize);
char	*ft_strdup(const char *s1);
int	    ft_strlen(const char *str);
int     get_operator_token(char *input, int *i, t_token **head);
int    is_operator(char c);
t_token *new_token(t_token_type type, char *value, t_quote_type quote, int no_space);                                                                     
void    add_token(t_token **head, t_token *new);                               
t_token *lexer(char *input);                                                   
void    print_tokens(t_token *head);                                           
                                                                                 
// Fonctions operator                                                          
int     is_operator(char c);                                                   
int     get_operator_token(char *input, int *i, t_token **head);
int     handle_single_quote(char *input, int *i, t_token **head);
int    handle_double_quote(char *input, int *i, t_token **head);


#endif
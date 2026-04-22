#include "../../Includes/parser.h"
#include "../../Includes/minishell.h"
#include "../../Includes/lexer.h"

static int	ft_numlen(int n)
{
	int	len;

	len = (n <= 0);
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	nbr;

	nbr = n;
	len = ft_numlen(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nbr < 0)
		nbr = -nbr;
	while (len--)
	{
		str[len] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i++] = s2[j++];
	}
	str[i] = '\0';
	return (str);
}


void    expand_field(char **field, int exit_status)                                                                                                    
{                                                                                                                                                      
    char *old;                                                                                                                                         
                                                                                                                                                         
    if (*field != NULL)                                                                                                                                
    {                                                                                                                                                  
        old = *field;                                                                                                                                  
        *field = expand_str(old, exit_status);                                                                                                         
        free(old);                                                                                                                                     
    }                                                                                                                                                  
}   

void    expand_cmds(t_cmd *cmds, int exit_status)                                                                                                      
{                                                                                                                                                      
    int i;                                                                                                                                             
                                                                                                                                                         
    while (cmds != NULL)                                                                                                                               
    {                                                                                                                                                  
        if (cmds->args != NULL)                                                                                                                        
        {                                                                                                                                              
            i = 0;                                                                                                                                     
            while (cmds->args[i] != NULL)                                                                                                              
            {                                                                                                                                          
                expand_field(&cmds->args[i], exit_status);                                                                                             
                i++;                                                                                                                                   
            }                                                                                                                                          
        }                                                                                                                                              
        expand_field(&cmds->redir_in, exit_status);                                                                                                    
        expand_field(&cmds->redir_out, exit_status);                                                                                                   
        cmds = cmds->next;                                                                                                                             
    }                                                                                                                                                  
}   

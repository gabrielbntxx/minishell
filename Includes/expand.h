#ifndef EXPAND_H
#define EXPAND_H

#include "minishell.h"

int     ft_strlcat(char *dst, const char *src, int size);
int	    expand_one_arg(char **arg, t_env **env);
void	expand_tokens(t_token *tok, t_env **env);
void	merge_tokens(t_token *tok);



#endif

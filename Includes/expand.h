#ifndef EXPAND_H
#define EXPAND_H

#include "minishell.h"

int ft_strlcat(char *dst, const char *src, int size);
void expand(t_cmd *cmd, t_shell *sh);

#endif

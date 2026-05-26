#include "../../Includes/expand.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
	{
		return (8);
	}
	else
		return (0);
}

int	ft_strlcat(char *dst, const char *src, int size)
{
	int	len;
	int	i;
	int	j;
	int	size2;

	size2 = size;
	len = 0;
	i = 0;
	j = 0;
	i = ft_strlen(dst);
	j = ft_strlen(src);
	if (size2 <= i)
		return (size2 + j);
	while (src[len] && i + len < size2 - 1)
	{
		dst[i + len] = src[len];
		len++;
	}
	dst[i + len] = '\0';
	return (i + j);
}


void expand(t_cmd *cmd, t_env *env) {
    int i;
    int y;
    int end; 
    char *value; 
    char *tmp; 
    char *post;

    tmp = NULL; 
    post = NULL;
    i = 0;
    end = 0;
    y = 0;
    while (cmd->args[i]) {
        if (ft_strchr(cmd->args[i], '$') != -1) {
            y = ft_strchr(cmd->args[i], '$');
            end = y + 1; 
            tmp = ft_substr(cmd->args[i], 0, y);
            if (cmd->args[i][y + 1] == '?') {
                return; //exit value  
            }
            while(ft_isalnum(cmd->args[i][end]))
              end++;
            value = ft_substr(cmd->args[i], y + 1, end - (y + 1));
            value = env_get(env, value, 0);
            post = ft_substr(cmd->args[i], end, ft_strlen(cmd->args[i] + end));
            if (!value) ft_strdup("");
            cmd->args[i] = ft_strjoin(tmp, value);
            //if (end - 1 > (int)ft_strlen(cmd->args[i]))
            cmd ->args[i] = ft_strjoin(cmd->args[i], post);
            //ft_strlcpy(&cmd->args[i][y], value, ft_strlen(value) + 1);
        }
        i++;
    }   
}

#include "../../Includes/expand.h"



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
    char *value; 


    i = 0;
    y = 0;
    while (cmd->args[i]) {
        if (ft_strchr(cmd->args[i], '$') != -1) {
            y = ft_strchr(cmd->args[i], '$');
            if (cmd->args[i][y] == '?') {
                return; //exit value
            }
            value = env_get(env, &cmd->args[i][y + 1], 0);
            if (!value) return;
            ft_strlcpy(&cmd->args[i][y], value, ft_strlen(value) + 1);
        }
        i++;
    }   
}

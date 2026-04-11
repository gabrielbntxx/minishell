
#include "res.h"


int     ft_strcmp(char *s1, char *s2)
{
    while (*s1 || *s2)
    {
        if (*s1 != *s2)
            return ((unsigned char)*s1 - (unsigned char)*s2);
        s1++;
        s2++;
    }
    return (0);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (s[j])
	{
		j++;
	}
	while (s[i] != (char)c && i <= j - 1)
	{
		i++;
	}
	if (s[i] == (char)c)
		return (&((char *)s)[i]);
	else
		return (NULL);
}


char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	size;

	if (!s)
		return (NULL);
	size = ft_strlen(s) - start;
	if (size > len)
		size = len;
	if (start >= ft_strlen(s))
	{
		dest = malloc(sizeof(char));
		if (!dest)
			return (0);
		dest[0] = 0;
		return (dest);
	}
	dest = malloc(sizeof(char) * (size + 1));
	if (!dest)
		return (0);
	ft_strlcpy(dest, &s[start], (size + 1));
	return (dest);
}


int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (2048);
	}
	else
		return (0);
}


int     is_numeric(char *str)
{
    int i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

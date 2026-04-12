#include "res.h"



size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	i = 0;
	if (size == 0)
		return (len);
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}


int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n && str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	if (i == n)
		return (0);
	return (str1[i] - str2[i]);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*s3;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	s3 = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!s3)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		s3[i] = s1[i];
		i++;
	}
	s3[i++] = '/';
	j = 0;
	while (s2[j])
	{
		s3[i + j] = s2[j];
		j++;
	}
	s3[i + j] = '\0';
	return (s3);
}


/*############################*/


static size_t	ft_count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	if (s[0] && s[0] != c)
		count++;
	while (s[++i])
		if (s[i] != c && s[i - 1] == c)
			count++;
	return (count);
}

static void	*ft_free(char **w, size_t j)
{
	while (j--)
		free(w[j]);
	free(w);
	return (NULL);
}

static size_t	ft_next_word(char const *s, char c, size_t i, char **w)
{
	size_t	start;

	while (s[i] && s[i] == c)
		i++;
	start = i;
	while (s[i] && s[i] != c)
		i++;
	*w = ft_substr(s, start, i - start);
	return (i);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	char	**w;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	w = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!w)
		return (NULL);
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (!s[i])
			break ;
		i = ft_next_word(s, c, i, &w[j]);
		if (!w[j++])
			return (ft_free(w, j - 1));
	}
	w[j] = NULL;
	return (w);
}

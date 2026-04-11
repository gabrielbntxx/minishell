#ifndef RES_H
  #define RES_h

#include "../../Includes/minishell.h"

size_t ft_strlen(const char *str);
size_t ft_strlcpy(char *dst, const char *src, size_t size);
char *ft_substr(char const *s, unsigned int start, size_t len);
int ft_strncmp(const char *s1, const char *s2, unsigned int n);
char *ft_strjoin(const char *s1, const char *s2);
char **ft_split(char const *s, char c);

char *ft_strchr(const char *s, int c);
char *ft_substr(char const *s, unsigned int start, size_t len);
int ft_isdigit(int c);
int is_numeric(char *str);

#endif

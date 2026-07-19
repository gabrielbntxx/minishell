/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielbenetrix <gabrielbenetrix@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:54:00 by mguilber          #+#    #+#             */
/*   Updated: 2026/07/14 00:00:00 by gabrielbene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RES_H
# define RES_H

# include <stddef.h>
# include <stdlib.h>

char	*ft_itoa(int n);
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strncmp(const char *s1, const char *s2, unsigned int n);
char	*ft_strjoin(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *src);
int		ft_strchr(const char *s, int c);
int		ft_isdigit(int c);
int		is_numeric(char *str);
int		ft_strcmp(char *s1, char *s2);

#endif

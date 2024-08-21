/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 15:51:10 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/18 15:57:55 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(const char *str, const char *accept)
{
	const char	*s;
	const char	*a;
	size_t		count;

	count = 0;
	s = str;
	while (*s)
	{
		a = accept;
		while (*a)
		{
			if (*s == *a)
				break ;
			++a;
		}
		if (*a == '\0')
			return (count);
		++count;
		++s;
	}
	return (count);
}

char	*ft_strpbrk(const char *str, const char *accept)
{
	const char	*s;
	const char	*a;

	s = str;
	while (*s)
	{
		a = accept;
		while (*a)
		{
			if (*s == *a)
				return ((char *)s);
			++a;
		}
		++s;
	}
	return (NULL);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*lasts;
	char		*start;
	char		*end;

	if (str == NULL)
		str = lasts;
	if (str == NULL)
		return (NULL);
	str += ft_strspn(str, delim);
	if (*str == '\0')
	{
		lasts = NULL;
		return (NULL);
	}
	start = str;
	end = ft_strpbrk(start, delim);
	if (end == NULL)
		lasts = NULL;
	else
	{
		*end = '\0';
		lasts = end + 1;
	}
	return (start);
}

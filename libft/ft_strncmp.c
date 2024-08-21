/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:47:44 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/17 15:27:10 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (ptr1[i] && ptr2[i] && ptr1[i] == ptr2[i])
	{
		if (i + 1 == n)
			break ;
		i++;
	}
	return (ptr1[i] - ptr2[i]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned int	n;
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	n = ft_strlen(s1);
	if (ft_strlen(s2) > (int)n)
		n = ft_strlen(s2);
	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (ptr1[i] && ptr2[i] && ptr1[i] == ptr2[i])
	{
		if (i + 1 == n)
			break ;
		i++;
	}
	return (ptr1[i] - ptr2[i]);
}

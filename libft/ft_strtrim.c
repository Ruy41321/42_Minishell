/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:49:15 by lpennisi          #+#    #+#             */
/*   Updated: 2024/01/19 22:59:35 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		y;
	char	*buffer;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j] && ft_strchr(set, s1[j]))
		j++;
	y = ft_strlen((char *) s1);
	while (y > j && ft_strchr(set, s1[y - 1]))
		y--;
	buffer = (char *) malloc(sizeof(*s1) * (y - j + 1));
	if (!buffer)
		return (NULL);
	while (j < y)
		buffer[i++] = s1[j++];
	buffer[i] = 0;
	return ((char *)buffer);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:22:33 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/24 18:22:39 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *to_copy)
{
	char		*copy;
	size_t		i;

	if (!to_copy)
	{
		copy = malloc(2 * sizeof(char));
		copy[0] = ' ';
		copy[1] = '\0';
		return (copy);
	}
	copy = malloc((ft_strlen(to_copy) + 1) * sizeof(char));
	i = 0;
	while (to_copy[i])
	{
		copy[i] = to_copy[i];
		i += 1;
	}
	copy[i] = '\0';
	return (copy);
}

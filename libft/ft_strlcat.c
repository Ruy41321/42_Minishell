/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:43:14 by lpennisi          #+#    #+#             */
/*   Updated: 2024/01/18 14:43:18 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	char	*dest_ptr;
	char	*start_ptr;
	size_t	dest_len;
	size_t	remaining;

	dest_ptr = dest;
	start_ptr = (char *) src;
	remaining = size;
	while (remaining-- != 0 && *dest_ptr != '\0')
		dest_ptr++;
	dest_len = dest_ptr - dest;
	remaining = size - dest_len;
	if (remaining == 0)
		return (dest_len + ft_strlen((char *) src));
	while (*src != '\0')
	{
		if (remaining > 1)
		{
			*dest_ptr++ = *src;
			remaining--;
		}
		src++;
	}
	*dest_ptr = '\0';
	return (dest_len + (src - start_ptr));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:48:00 by lpennisi          #+#    #+#             */
/*   Updated: 2024/01/18 14:48:01 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*buff1;
	unsigned char	*buff2;

	buff1 = (unsigned char *)dest;
	buff2 = (unsigned char *)src;
	if (!dest && !src)
		return (dest);
	if (src < dest)
		while (n--)
			buff1[n] = buff2[n];
	else
		while (n--)
			*buff1++ = *buff2++;
	return (dest);
}

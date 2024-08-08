/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 21:55:56 by lpennisi          #+#    #+#             */
/*   Updated: 2024/01/19 19:34:34 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*new_head;

	if (!lst || !f || !del)
		return (NULL);
	new_node = ft_lstnew(f(lst -> content));
	new_head = new_node;
	while (lst -> next)
	{
		lst = lst -> next;
		if (!(lst -> content))
			del(lst -> content);
		new_node -> next = ft_lstnew(f(lst -> content));
		if (!new_node -> next)
		{
			ft_lstclear(&new_head, del);
			return (NULL);
		}
		new_node = new_node -> next;
	}
	return (new_head);
}

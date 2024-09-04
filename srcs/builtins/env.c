/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:31 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/03 20:43:54 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_env_var *env)
{
	t_env_var	*head;

	head = env;
	while (head)
	{
		ft_putstr(head->name);
		ft_putchar('=');
		ft_putendl_fd(head->value, 1);
		head = head->next;
	}
}

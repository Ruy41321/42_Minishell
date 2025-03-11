/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:31 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/28 14:19:09 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_builtin(t_env_var *env)
{
	t_env_var	*head;

	head = env;
	while (head)
	{
		if (ft_strcmp(head->value, "") != 0)
		{
			ft_putstr(head->name);
			ft_putchar('=');
			ft_putendl_fd(head->value, 1);
		}
		head = head->next;
	}
	set_exit_status(0);
}

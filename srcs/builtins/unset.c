/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:46 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/28 14:20:54 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(t_env_var **head, char *name)
{
	t_env_var	*temp;
	t_env_var	*prev;

	temp = *head;
	prev = NULL;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				*head = temp->next;
			free(temp->name);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

int	unset_builtin(t_my_envp *my_envp, char **command)
{
	int			i;

	i = 1;
	while (command[i])
	{
		remove_env_var(&my_envp->locals, command[i]);
		remove_env_var(&my_envp->exported, command[i]);
		i++;
	}
	g_exit_status = 0;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:07:21 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/27 15:07:29 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_env_var(t_env_var *head, char *name, char *value, int create)
{
	value = remove_quotes(value);
	if (head->name == NULL)
	{
		head->name = ft_strdup(name);
		head->value = ft_strdup(value);
	}
	else
	{
		while (head->next != NULL && ft_strcmp(head->name, name))
			head = head->next;
		if (!ft_strcmp(head->name, name))
		{
			free(head->value);
			head->value = ft_strdup(value);
			return (free(value), 1);
		}
		else if (create)
		{
			head->next = safe_malloc(sizeof(t_env_var));
			head->next->name = ft_strdup(name);
			head->next->value = ft_strdup(value);
			head->next->next = NULL;
		}
	}
	return (free(value), 0);
}

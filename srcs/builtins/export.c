/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:40 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/13 17:37:36 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export(t_env_var *head)
{
	while (head != NULL)
	{
		if (head->value != NULL)
		{
			ft_putstr_fd(head->name, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(head->value, 1);
			ft_putstr_fd("\n", 1);
		}
		head = head->next;
	}
}

static int	delete_local_var(t_env_var *head, char *name)
{
	t_env_var	*tmp;
	t_env_var	*prev;

	tmp = head;
	prev = NULL;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (prev == NULL)
				head = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

static void	export_existing(t_my_envp *my_envp, char *name)
{
	char	*value;

	value = get_env_var(my_envp->locals, name);
	if (value)
	{
		set_env_var(my_envp->exported, name, value, 1);
		delete_local_var(my_envp->locals, name);
	}
}

int	export_builtin(t_my_envp *my_envp, char **command)
{
	char	*equals_sign;
	char	*value;
	char	*name;

	if (command[1] == NULL)
		return (print_export(my_envp->exported), 0);
	equals_sign = ft_strchr(command[1], '=');
	if (equals_sign == NULL)
		return (export_existing(my_envp, command[1]), 0);
	*equals_sign = '\0';
	name = command[1];
	name = remove_quotes(name);
	if (get_delim(name))
	{
		*equals_sign = '=';
		free(name);
		return (1);
	}
	value = equals_sign + 1;
	if (get_env_var(my_envp->locals, name))
		delete_local_var(my_envp->locals, name);
	set_env_var(my_envp->exported, name, value, 1);
	free(name);
	return (0);
}

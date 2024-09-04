/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:40 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/04 22:27:14 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_env_var *head)
{
	t_env_var	*tmp;

	tmp = head;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(tmp->name, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		tmp = tmp->next;
	}
}

int	delete_local_var(t_env_var *head, char *name)
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

int	export_builtin(t_my_envp *my_envp, char **command)
{
	char	*equals_sign;
	char	*value;
	char	*name;

	if (command[1] == NULL)
		return (print_export(my_envp->exported), 0);
	equals_sign = ft_strchr(command[1], '=');
	if (equals_sign == NULL)
		return (1);
	*equals_sign = '\0';
	name = command[1];
	if (get_delim(name))
	{
		*equals_sign = '=';
		return (1);
	}
	value = equals_sign + 1;
	if (get_env_var(my_envp->locals, name))
		delete_local_var(my_envp->locals, name);
	set_env_var(my_envp->exported, name, value, 1);
	return (0);
}

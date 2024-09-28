/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:40 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/28 14:20:30 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env_matrix(char **env_matrix, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(env_matrix[i], env_matrix[j]) > 0)
			{
				tmp = env_matrix[i];
				env_matrix[i] = env_matrix[j];
				env_matrix[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export(t_env_var *head)
{
	char	**env_matrix;
	int		size;
	int		i;

	env_matrix = list_to_matrix(head, 0);
	size = matrixlen(env_matrix);
	sort_env_matrix(env_matrix, size);
	i = 0;
	while (env_matrix[i] != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env_matrix[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	free_command(env_matrix, size);
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

	if ((!ft_isalpha(name[0]) && name[0] != '_') || get_delim(name))
	{
		builtin_error("export", name, "not a valid identifier");
		set_exit_status(1);
		return ;
	}
	value = get_env_var(my_envp->locals, name);
	if (value)
	{
		set_env_var(my_envp->exported, name, value, 1);
		delete_local_var(my_envp->locals, name);
	}
	else
		set_env_var(my_envp->exported, name, "", 1);
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
	if (get_delim(name) || !ft_isalpha(name[0]))
	{
		*equals_sign = '=';
		builtin_error("export", command[1], "not a valid identifier");
		return (set_exit_status(1), free(name), 1);
	}
	value = equals_sign + 1;
	if (get_env_var(my_envp->locals, name))
		delete_local_var(my_envp->locals, name);
	set_env_var(my_envp->exported, name, value, 1);
	free(name);
	g_exit_status = 0;
	return (0);
}

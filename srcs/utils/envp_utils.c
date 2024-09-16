/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:01:50 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/17 00:57:10 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_var_list(t_env_var *head)
{
	t_env_var	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	free_my_envp(t_my_envp *my_envp)
{
	if (my_envp->exported)
		free_env_var_list(my_envp->exported);
	if (my_envp->locals)
		free_env_var_list(my_envp->locals);
	free(my_envp);
}

void	increase_shlvl(t_env_var *head)
{
	char	*shlvl;
	int		lvl;

	shlvl = get_env_var(head, "SHLVL");
	if (shlvl == NULL)
	{
		set_env_var(head, "SHLVL", "1", 1);
		return ;
	}
	lvl = ft_atoi(shlvl);
	lvl++;
	shlvl = ft_itoa(lvl);
	set_env_var(head, "SHLVL", shlvl, 1);
	free(shlvl);
}

char	*get_env_name(char *envp)
{
	char	*name;
	int		i;

	i = 0;
	while (envp[i] != '=')
		i++;
	name = safe_malloc(i + 1);
	ft_strlcpy(name, envp, i + 1);
	return (name);
}

int	matrixlen(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i] != NULL)
		i++;
	return (i);
}

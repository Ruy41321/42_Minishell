/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:05:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 22:16:50 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	g_exit_status = 0;

void	free_my_envp(t_my_envp *my_envp)
{
	t_env_var	*head;
	t_env_var	*tmp;
	int			i;

	i = -1;
	head = my_envp->exported;
	while (++i < 2)
	{
		if (head)
		{
			while (head->next != NULL)
			{
				tmp = head;
				head = head->next;
				free(tmp->name);
				free(tmp->value);
				free(tmp);
			}
			if (head->name != NULL)
			{
				free(head->name);
				free(head->value);
			}
			free(head);
		}
		if (i == 0)
			head = my_envp->locals;
	}
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

t_env_var	*get_head(void)
{
	t_env_var	*head;
	head = safe_malloc(sizeof(t_env_var));
	head->name = ft_strdup("!head");
	head->value = ft_strdup("0");
	head->next = NULL;
	return (head);
}

t_my_envp	*init_envp(char **envp)
{
	t_my_envp	*my_envp;
	t_env_var	*head;
	t_env_var	*tmp;
	int			i;

	head = safe_malloc(sizeof(t_env_var));
	head->name = get_env_name(envp[0]);
	head->value = ft_strdup(getenv(head->name));
	head->next = NULL;
	tmp = head;
	i = 1;
	while (envp[i] != NULL)
	{
		tmp->next = safe_malloc(sizeof(t_env_var));
		tmp = tmp->next;
		tmp->name = get_env_name(envp[i]);
		tmp->value = ft_strdup(getenv(tmp->name));
		tmp->next = NULL;
		i++;
	}
	my_envp = safe_malloc(sizeof(t_my_envp));
	my_envp->locals = get_head();
	my_envp->exported = head;
	return (my_envp);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_my_envp	*my_envp;

	if (argc > 1 && argv)
	{
		ft_putstr_fd("Error: too many arguments\n", 2);
		return (1);
	} 
	my_envp = init_envp(envp);
	increase_shlvl(my_envp->exported);
	while (1)
	{
		signal_init();
		input = get_input(my_envp);
		if (!input)
			break ;
		parse_and_exec(my_envp, input);
	}
	free_my_envp(my_envp);
	ft_printf("exit\n");
	return (0);
}

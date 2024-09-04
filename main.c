/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:05:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 16:07:56 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	g_exit_status = 0;

void	free_head(t_env_var *head)
{
	t_env_var	*tmp;

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

void	increase_shlvl(t_env_var *head)
{
	char	*shlvl;
	int		lvl;

	shlvl = get_env_var(head, "SHLVL");
	if (shlvl == NULL)
	{
		set_env_var(head, "SHLVL", "1");
		return ;
	}
	lvl = ft_atoi(shlvl);
	lvl++;
	shlvl = ft_itoa(lvl);
	set_env_var(head, "SHLVL", shlvl);
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

t_env_var	*init_head(char **envp)
{
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
	return (head);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_env_var	*head;

	if (argc > 1 && argv)
	{
		ft_putstr_fd("Error: too many arguments\n", 2);
		return (1);
	}
	head = init_head(envp);
	increase_shlvl(head);
	while (1)
	{
		signal_init();
		input = get_input(head);
		if (!input)
			break ;
		parse_and_exec(head, input);
	}
	free_head(head);
	ft_printf("exit\n");
	return (0);
}

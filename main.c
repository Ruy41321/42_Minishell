/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:05:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/28 16:59:58 by lpennisi         ###   ########.fr       */
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

t_env_var	*get_head(void)
{
	t_env_var	*head;

	head = safe_malloc(sizeof(t_env_var));
	head->name = NULL;
	head->value = NULL;
	head->next = NULL;
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
	head = get_head();
	while (1)
	{
		signal_init();
		input = get_input(envp);
		if (!input)
			break ;
		parse_and_exec(head, input, envp);
	}
	free_head(head);
	ft_printf("exit\n");
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:05:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/20 15:16:01 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	g_exit_status = 0;

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

void	clean_tmp_files(void)
{
	int		i;
	char	*filename;

	i = 0;
	while (1)
	{
		filename = ft_strjoin_free("/tmp/heredoc", ft_itoa(i), 2);
		if (access(filename, F_OK) == -1)
		{
			free(filename);
			break ;
		}
		unlink(filename);
		free(filename);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_my_envp	*my_envp;
	t_parser	parser;

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
		parser.input = get_input(my_envp);
		if (!parser.input)
			break ;
		parse_and_exec(my_envp, &parser);
	}
	clean_tmp_files();
	free_my_envp(my_envp);
	ft_printf("exit\n");
	return (0);
}

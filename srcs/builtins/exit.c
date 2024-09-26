/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:35 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/26 15:54:25 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	str = remove_quotes(str);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (free(str), 0);
		i++;
	}
	return (free(str), 1);
}

static void	safe_exit(t_my_envp *my_envp, char **command)
{
	free_command(command, -1);
	free_my_envp(my_envp);
	exit(g_exit_status);
}

int	exit_builtin(t_my_envp *my_envp, char **command)
{
	char	*temp;

	if (command[1])
	{
		temp = remove_quotes(command[1]);
		if (ft_isnumber(temp))
		{
			g_exit_status = ft_atoi(temp);
			if (command[2])
			{
				ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2);
				return (set_exit_status(1), free(temp), 1);
			}
		}
		else
		{
			ft_putstr_fd("exit\nminishell: exit: ", 2);
			ft_putstr_fd(command[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_exit_status = 2;
			safe_exit(my_envp, command);
		}
		free(temp);
	}
	return (safe_exit(my_envp, command), 0);
}

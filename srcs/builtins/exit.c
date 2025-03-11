/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:35 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/27 15:18:21 by flo-dolc         ###   ########.fr       */
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
	return (ft_putstr_fd("exit\n", 2), safe_exit(my_envp, command), 0);
}

void	execve_error(char *full_path, char **list, char **piped_command)
{
	ft_putstr_fd("minishell: ", 2);
	perror(piped_command[0]);
	free_command(list, -1);
	free_command(piped_command, -1);
	free(full_path);
	exit(2);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:27 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/09 02:03:26 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_n_flag(char **command, int *n_flag)
{
	int	i;
	int	j;

	i = 1;
	j = 1;
	if (command[i] && command[i][0] == '-' && command[i][1] == 'n')
	{
		while (command[i] && command[i][j] == 'n')
			j++;
		if (command[i][j] == '\0')
		{
			*n_flag = 1;
			i++;
		}
	}
	return (i);
}

int	echo_builtin(char **command)
{
	int		i;
	int		n_flag;
	char	*tmp;

	n_flag = 0;
	i = check_n_flag(command, &n_flag);
	while (command[i])
	{
		tmp = remove_quotes(command[i]);
		ft_putstr_fd(tmp, 1);
		free(tmp);
		if (command[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}

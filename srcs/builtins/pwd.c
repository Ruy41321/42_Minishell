/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:43 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/26 15:27:54 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_error(char *builtin, char *value, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(builtin, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd("': ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

void	set_exit_status(int status)
{
	g_exit_status = status;
}

int	pwd_builtin(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	return (1);
}

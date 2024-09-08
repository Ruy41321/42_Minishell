/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:04 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/07 19:02:04 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_builtin(t_my_envp *my_envp, char **command)
{
	char	*path;
	char	*oldpwd;
	char	*pwd;

	if (command[1] == NULL)
	{
		path = get_env_var(my_envp->exported, "HOME");
		if (path == NULL)
			return (1);
	}
	else
		path = command[1];
	oldpwd = get_env_var(my_envp->exported, "PWD");
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	set_env_var(my_envp->exported, "OLDPWD", oldpwd, 1);
	set_env_var(my_envp->exported, "PWD", pwd, 1);
	free(pwd);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:04 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/15 01:31:40 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_chdir_allowed(char *path)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

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
	else if (command[2] != NULL)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	else
		path = command[1];
	oldpwd = get_env_var(my_envp->exported, "PWD");
	if (is_chdir_allowed(path))
		return (1);
	pwd = getcwd(NULL, 0);
	set_env_var(my_envp->exported, "OLDPWD", oldpwd, 1);
	set_env_var(my_envp->exported, "PWD", pwd, 1);
	free(pwd);
	return (1);
}

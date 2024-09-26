/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:26:04 by flo-dolc          #+#    #+#             */
/*   Updated: 2024/09/26 15:01:01 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_tilde(char *path, t_env_var *env, int *mallocated)
{
	char	*home;

	if (path[0] == '~')
	{
		home = get_env_var(env, "HOME");
		if (home == NULL)
			return (path);
		*mallocated = 1;
		return (ft_strjoin(home, path + 1));
	}
	*mallocated = 0;
	return (path);
}

static int	is_chdir_allowed(char *path, int mallocated)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		if (mallocated)
			free(path);
		return (1);
	}
	if (mallocated)
		free(path);
	return (0);
}

static int	arg_check(t_my_envp *my_envp, char **command, char **path)
{
	*path = command[1];
	if (command[1] == NULL || ft_strcmp(command[1], "~") == 0)
	{
		*path = get_env_var(my_envp->exported, "HOME");
		if (*path == NULL)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if (command[2] != NULL)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	else if (ft_strcmp(command[1], "-") == 0)
	{
		*path = get_env_var(my_envp->exported, "OLDPWD");
		if (*path == NULL)
			*path = get_env_var(my_envp->locals, "OLDPWD");
		if (*path == NULL)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
	}
	return (0);
}

static void	set_pwd(t_my_envp *my_envp, char *pwd, char *oldpwd)
{
	if (get_env_var(my_envp->exported, "OLDPWD") == NULL)
	{
		if (oldpwd != NULL)
			set_env_var(my_envp->locals, "OLDPWD", oldpwd, 1);
	}
	else
		set_env_var(my_envp->exported, "OLDPWD", oldpwd, 1);
	if (get_env_var(my_envp->exported, "PWD") == NULL)
		set_env_var(my_envp->locals, "PWD", pwd, 1);
	else
		set_env_var(my_envp->exported, "PWD", pwd, 1);
}

int	cd_builtin(t_my_envp *my_envp, char **command)
{
	char	*path;
	char	*oldpwd;
	char	*pwd;
	int		mallocated;

	if (arg_check(my_envp, command, &path))
		return (set_exit_status(1), 1);
	oldpwd = get_env_var(my_envp->exported, "PWD");
	if (oldpwd == NULL)
		oldpwd = get_env_var(my_envp->locals, "PWD");
	path = replace_tilde(path, my_envp->exported, &mallocated);
	if (is_chdir_allowed(path, mallocated))
		return (set_exit_status(1), 1);
	if (command[1] && ft_strcmp(command[1], "-") == 0)
		ft_putendl_fd(path, 1);
	pwd = getcwd(NULL, 0);
	set_pwd(my_envp, pwd, oldpwd);
	free(pwd);
	return (0);
}

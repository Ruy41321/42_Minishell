/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 19:01:24 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 16:12:09 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var(t_env_var *env, const char *var_name)
{
	t_env_var	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->name, var_name, ft_strlen(var_name)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	is_path(char *command)
{
	if (ft_strchr(command, '/') != NULL)
		return (1);
	return (0);
}

char	*look_for_file(char *command, char **path_tokens)
{
	int		i;
	char	*temp_path;
	char	*ret;

	i = -1;
	while (path_tokens[++i] != NULL)
	{
		temp_path = ft_strjoin(path_tokens[i], "/");
		ret = ft_strjoin(temp_path, command);
		free(temp_path);
		if (access(ret, F_OK) == 0)
			return (ret);
		free(ret);
	}
	return (NULL);
}

char	*get_full_path(char *command, t_env_var *env)
{
	char	*ret;
	char	*path;
	char	**path_tokens;
	int		j;

	if (is_path(command))
		return (ft_strdup(command));
	ret = NULL;
	path = get_env_var(env, "PATH");
	if (!path)
		return (NULL);
	path = ft_strdup(path);
	path_tokens = ft_split(path, ':');
	free(path);
	ret = look_for_file(command, path_tokens);
	j = 0;
	while (path_tokens[j] != NULL)
		free(path_tokens[j++]);
	free(path_tokens);
	return (ret);
}

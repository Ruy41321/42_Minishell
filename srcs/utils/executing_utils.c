/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:45:33 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/12 12:43:17 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_stdfd(int *fd)
{
	if (!fd[0] || !fd[1])
	{
		fd[0] = dup(STDIN_FILENO);
		fd[1] = dup(STDOUT_FILENO);
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
	}
}

int	handle_pipe(char **piped_command, int *p_fd, int *is_prep)
{
	if (*is_prep)
		return (1);
	*is_prep = is_prepipe_command(piped_command);
	if (*is_prep)
	{
		if (pipe(p_fd) == -1)
		{
			perror("Pipe failed");
			exit(1);
		}
		return (1);
	}
	return (0);
}

int	handle_env_var_assignment(t_my_envp *my_envp, char **piped_command)
{
	char	*equals_sign;
	char	*value;
	char	*name;

	equals_sign = ft_strchr(*piped_command, '=');
	if (equals_sign != NULL)
	{
		*equals_sign = '\0';
		name = *piped_command;
		if (get_delim(name))
		{
			*equals_sign = '=';
			return (0);
		}
		value = equals_sign + 1;
		if (!set_env_var(my_envp->exported, name, value, 0))
			set_env_var(my_envp->locals, name, value, 1);
		return (1);
	}
	return (0);
}

char	**list_to_matrix(t_env_var *head)
{
	char		**envp;
	int			i;
	t_env_var	*tmp;

	i = 0;
	tmp = head;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	envp = safe_malloc(sizeof(char *) * (i + 1));
	i = 0;
	tmp = head;
	while (tmp != NULL)
	{
		envp[i] = ft_strjoin(tmp->name, "=");
		envp[i] = ft_strjoin_free(envp[i], tmp->value, 1);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

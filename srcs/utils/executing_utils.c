/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:45:33 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 16:35:58 by flo-dolc         ###   ########.fr       */
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

void	handle_pipe(char **piped_command, int *p_fd, int *is_prep, int std_out)
{
	if (*is_prep)
	{
		dup2(p_fd[0], STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
	}
	close(p_fd[0]);
	*is_prep = is_prepipe_command(piped_command);
	if (*is_prep)
	{
		if (pipe(p_fd) == -1)
		{
			perror("Pipe failed");
			exit(1);
		}
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
	}
}

int	handle_env_var_assignment(t_env_var *head, char **piped_command)
{
	char	*equals_sign;
	char	*value;
	char	*name;

	equals_sign = ft_strchr(*piped_command, '=');
	if (equals_sign != NULL)
	{
		*equals_sign = '\0';
		name = *piped_command;
		if (ft_strchr(*piped_command, '"') || ft_strchr(*piped_command, '\''))
		{
			*equals_sign = '=';
			return (0);
		}
		value = equals_sign + 1;
		set_env_var(head, name, value);
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
		envp[i] = ft_strjoin(envp[i], tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

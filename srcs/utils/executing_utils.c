/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:45:33 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/27 14:52:47 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_stdfd(int **fd)
{
	if (!*fd)
	{
		*fd = safe_malloc(sizeof(int) * 2);
		(*fd)[0] = dup(STDIN_FILENO);
		(*fd)[1] = dup(STDOUT_FILENO);
	}
	else
	{
		dup2((*fd)[0], STDIN_FILENO);
		dup2((*fd)[1], STDOUT_FILENO);
	}
}

int	handle_pipe(char **piped_command, int *p_fd, int *is_prep)
{
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
		if ((!ft_isalpha(name[0]) && name[0] != '_') || get_delim(name))
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

char	**list_to_matrix(t_env_var *head, int check_empty)
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
	while (head != NULL)
	{
		if (ft_strcmp(head->value, "") != 0 || !check_empty)
		{
			envp[i] = ft_strjoin(head->name, "=");
			envp[i] = ft_strjoin_free(envp[i], head->value, 1);
			i++;
		}
		head = head->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	exe_bultin(t_my_envp *my_envp, char **command)
{
	if (handle_env_var_assignment(my_envp, command))
		return (1);
	if (ft_strcmp(command[0], "cd") == 0)
		return (cd_builtin(my_envp, command), 1);
	if (ft_strcmp(command[0], "echo") == 0)
		return (echo_builtin(command), 1);
	if (ft_strcmp(command[0], "env") == 0)
		return (env_builtin(my_envp->exported), 1);
	if (ft_strcmp(command[0], "exit") == 0)
		return (exit_builtin(my_envp, command), 1);
	if (ft_strcmp(command[0], "export") == 0)
		return (export_builtin(my_envp, command), 1);
	if (ft_strcmp(command[0], "pwd") == 0)
		return (pwd_builtin(), 1);
	if (ft_strcmp(command[0], "unset") == 0)
		return (unset_builtin(my_envp, command), 1);
	return (0);
}

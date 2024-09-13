/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/13 12:25:47 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(char **piped_command, t_my_envp *envp)
{
	char	*full_path;

	signal(SIGINT, signal_handler_child);
	if (!piped_command)
		exit(1);
	remove_quotes_2d(piped_command);
	full_path = get_full_path(piped_command[0], envp->exported);
	if (!full_path)
	{
		full_path = ft_strjoin(piped_command[0], ": command not found\n");
		ft_putstr_fd(full_path, 2);
		free(full_path);
		free_command(piped_command, -1);
		exit(127);
	}
	if (execve(full_path, piped_command, list_to_matrix(envp->exported)) == -1)
	{
		perror(piped_command[0]);
		free_command(piped_command, -1);
		free(full_path);
		exit(2);
	}
}

int	parent_process(pid_t ch)
{
	int	child_status;

	signal(SIGINT, SIG_IGN);
	waitpid(ch, &child_status, WUNTRACED);
	if (WIFEXITED(child_status))
		g_exit_status = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
	{
		g_exit_status = 128 + WTERMSIG(child_status);
		if (WTERMSIG(child_status) == SIGINT)
			ft_printf("\n");
		return (1);
	}
	else
	{
		ft_printf("Debug:: questo caso non dovrebbe mai accadere\n");
		g_exit_status = child_status >> 8;
	}
	return (0);
}

int	handle_exe(char **command, t_my_envp *my_envp)
{
	pid_t	child;

	child = fork();
	if (child < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	if (child == 0)
		child_process(command, my_envp);
	return (parent_process(child));
}

int	execute_handler(t_my_envp *my_envp, char **command)
{
	char	***piped_command;
	int		origin_std[2];
	pid_t	*child;
	int		i;

	handle_stdfd(origin_std);
	piped_command = get_piped_command(command);
	if (ft_3d_arrlen(piped_command) > 1)
	{
		child = safe_malloc(sizeof(pid_t) * (ft_3d_arrlen(piped_command)));
		exe_pipe(my_envp, piped_command, NULL, child);
		i = -1;
		while (++i < ft_3d_arrlen(piped_command))
			parent_process(child[i]);
		free(child);
	}
	else
	{
		*piped_command = handle_redirection(*piped_command);
		if (!exe_bultin(my_envp, *piped_command))
			handle_exe(*piped_command, my_envp);
	}
	handle_stdfd(origin_std);
	return (free_command_3d(piped_command), 0);
}

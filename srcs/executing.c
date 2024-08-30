/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/30 14:53:43 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_exit_status;

void	child_process(char **piped_command, char **envp)
{
	char	*full_path;

	signal(SIGINT, SIG_DFL);
	piped_command = handle_redirection(piped_command);
	if (!piped_command)
		exit(0);
	remove_quotes_2d(piped_command);
	full_path = get_full_path(piped_command[0], envp);
	if (!full_path)
	{
		full_path = ft_strjoin(piped_command[0], ": command not found\n");
		ft_putstr_fd(full_path, 2);
		free(full_path);
		free_command(piped_command, -1);
		exit(1);
	}
	if (execve(full_path, piped_command, envp) == -1)
	{
		perror(piped_command[0]);
		free_command(piped_command, -1);
		free(full_path);
		exit(1);
	}
}

long long	parent_process(char ***cmd_ptr, char **command, pid_t ch, int *fd)
{
	int	child_status;

	signal(SIGINT, SIG_IGN);
	waitpid(ch, &child_status, WUNTRACED);
	if (WTERMSIG(child_status))
	{
		g_exit_status = child_status >> 8;
		handle_stdfd(fd);
		while (command != NULL)
		{
			free_command(command, -1);
			command++;
		}
		free(cmd_ptr);
		return (g_exit_status);
	}
	return (0);
}

long long	handle_exe(char ***cmd_ptr, char **command, char **envp, int *fd)
{
	pid_t	child;

	child = fork();
	if (child < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	if (child == 0)
		child_process(command, envp);
	else
		if (parent_process(cmd_ptr, command, child, fd))
			return (g_exit_status);
	return (0);
}

int	execute_command(t_env_var *head, char **command, char **envp)
{
	char	***piped_command;
	char	***piped_command_ptr;
	int		origin_std[2];
	int		pipefd[2];
	int		is_prepipe;

	handle_stdfd(origin_std);
	piped_command = get_piped_command(command);
	piped_command_ptr = piped_command;
	is_prepipe = 0;
	while (*piped_command != NULL)
	{
		handle_pipe(*piped_command, pipefd, &is_prepipe, origin_std[1]);
		if (!handle_env_var_assignment(head, *piped_command))
			if (handle_exe(piped_command_ptr, *piped_command, envp, origin_std))
				return (g_exit_status);
		free_command(*piped_command, -1);
		piped_command++;
	}
	handle_stdfd(origin_std);
	free(piped_command_ptr);
	return (0);
}

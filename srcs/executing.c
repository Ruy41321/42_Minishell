/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 16:11:23 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_exit_status;

void	child_process(char **piped_command, t_env_var *env)
{
	char	*full_path;

	signal(SIGINT, signal_handler_child);
	piped_command = handle_redirection(piped_command);
	if (!piped_command)
		exit(1);
	remove_quotes_2d(piped_command);
	full_path = get_full_path(piped_command[0], env);
	if (!full_path)
	{
		full_path = ft_strjoin(piped_command[0], ": command not found\n");
		ft_putstr_fd(full_path, 2);
		free(full_path);
		free_command(piped_command, -1);
		exit(127);
	}
	if (execve(full_path, piped_command, list_to_matrix(env)) == -1)
	{
		perror(piped_command[0]);
		free_command(piped_command, -1);
		free(full_path);
		exit(2);
	}
}

int	parent_process(pid_t ch, int *fd)
{
	int	child_status;

	signal(SIGINT, SIG_IGN);
	waitpid(ch, &child_status, WUNTRACED);
	handle_stdfd(fd);
	if (WIFEXITED(child_status))
		g_exit_status = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
	{
		g_exit_status = 128 + WTERMSIG(child_status);
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

int	handle_exe(char **command, t_env_var *env, int *fd)
{
	pid_t	child;

	child = fork();
	if (child < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	if (child == 0)
		child_process(command, env);
	else
		if (parent_process(child, fd))
			return (1);
	return (0);
}

int	execute_command(t_env_var *head, char **command)
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
		{
			if (handle_exe(*piped_command, head, origin_std))
			{
				free_command_3d(piped_command);
				return (free(piped_command_ptr), 1);
			}
		}
		free_command(*(piped_command++), -1);
	}
	handle_stdfd(origin_std);
	return (free(piped_command_ptr), 0);
}

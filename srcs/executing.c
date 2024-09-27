/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/27 14:37:38 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(char **piped_command, t_my_envp *envp)
{
	char	*full_path;
	char	**list;
	int		exit_status;

	signal(SIGINT, signal_handler_child);
	if (!piped_command)
		exit(1);
	remove_quotes_2d(piped_command);
	full_path = get_full_path(piped_command[0], envp->exported);
	exit_status = handle_wrong_exe(full_path, piped_command[0]);
	if (exit_status)
	{
		if (full_path)
			free(full_path);
		free_command(piped_command, -1);
		exit(exit_status);
	}
	list = list_to_matrix(envp->exported, 1);
	if (execve(full_path, piped_command, list) == -1)
		execve_error(full_path, list, piped_command);
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
			return (ft_printf("\n"), 1);
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

int	run_pipe(t_my_envp *my_envp, char ***piped_command)
{
	pid_t	*child;
	int		status;
	int		i;

	status = 0;
	child = safe_malloc(sizeof(pid_t) * (ft_3d_arrlen(piped_command)));
	exe_pipe(my_envp, piped_command, NULL, child);
	i = -1;
	while (++i < ft_3d_arrlen(piped_command))
	{
		if (status)
			kill(child[i], SIGKILL);
		else
			status = parent_process(child[i]);
	}
	free(child);
	return (status);
}

int	execute_handler(t_my_envp *my_envp, char **command)
{
	char	***piped_command;
	int		*origin_std;
	int		status;

	origin_std = NULL;
	status = 0;
	handle_stdfd(&origin_std);
	command = handle_heredoc(command);
	if (!command)
		return (free(origin_std), 1);
	piped_command = get_piped_command(command);
	if (ft_3d_arrlen(piped_command) > 1)
		status = run_pipe(my_envp, piped_command);
	else
	{
		*piped_command = handle_redirection(*piped_command);
		if (!*piped_command)
			status = 1;
		else if (!exe_bultin(my_envp, *piped_command))
			status = handle_exe(*piped_command, my_envp);
	}
	handle_stdfd(&origin_std);
	free(origin_std);
	return (free_command_3d(piped_command), status);
}

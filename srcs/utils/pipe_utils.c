/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:23:25 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/15 01:27:14 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(t_my_envp *my_envp, char ***command)
{
	*command = handle_redirection(*command);
	if (*command)
		if (!exe_bultin(my_envp, *command))
			child_process(*command, my_envp);
	free_command(*command, -1);
	exit(0);
}

void	apply_pipe(int *pipefd, int in_out)
{
	if (in_out == 0)
		dup2(pipefd[0], STDIN_FILENO);
	else
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	close_pipe(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	exe_pipe(t_my_envp *envp, char ***commands, int *old_pipe, pid_t *ch)
{
	int		pipefd[2];
	int		is_prepipe;

	is_prepipe = 0;
	handle_pipe(commands[0], pipefd, &is_prepipe);
	*ch = fork();
	if (*ch == 0)
	{
		if (old_pipe)
			apply_pipe(old_pipe, 0);
		if (is_prepipe)
			apply_pipe(pipefd, 1);
		execute_child(envp, commands);
	}
	if (old_pipe)
		close_pipe(old_pipe);
	if (commands[1] != NULL)
		exe_pipe(envp, commands + 1, pipefd, ch + 1);
	if (is_prepipe)
		close_pipe(pipefd);
	return (1);
}

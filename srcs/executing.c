/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/12 16:37:24 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return(parent_process(child));
}

void	execute_child(t_my_envp *my_envp, char ***command)
{
	*command = handle_redirection(*command);
	if (!exe_bultin(my_envp, *command))
		child_process(*command, my_envp);
	free_command(*command, -1);
	exit(0);
}

int	execute_pipe(t_my_envp *my_envp, char ***pipe_command)
{
	int		origin_std[2];
	int		pipefd[2];
	int 	is_prepipe;
	int		i;
	pid_t	*child;

	is_prepipe = 0;
	handle_stdfd(origin_std);
	if (!handle_pipe(pipe_command[0], pipefd, &is_prepipe))
		return (0);
	child = safe_malloc(sizeof(pid_t) * (ft_3d_arrlen(pipe_command) + 1));
	i = -1;
	while (pipe_command[++i] != NULL)
	{
		child[i] = fork();
		if (child[i] == 0)
		{
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execute_child(my_envp, pipe_command + i);
		}
		free_command(pipe_command[i], -1);
		child[++i] = fork();
		if (child[i] == 0)
		{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execute_child(my_envp, pipe_command + i);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		free_command(pipe_command[i], -1);
	}
	child[i] = 0;
	i = -1;
	while (child[++i])
		parent_process(child[i]);
	handle_stdfd(origin_std);
	free(child);
	return (1);
}

// if (ft_3d_arrlen(pipe_command) == 1)
// 		return (0);
// 	child = safe_malloc(sizeof(pid_t) * (ft_3d_arrlen(pipe_command) + 1));
// 	i = -1;
// 	is_prepipe = handle_pipe(pipe_command[0], pipefd, &is_prepipe);
// 	while (pipe_command[++i] != NULL)
// 	{
// 		if (is_postpipe)
// 		{
// 			dup2(pipefd[0], STDIN_FILENO);
// 			close(pipefd[0]);
// 			dup2(origin_std[1], STDOUT_FILENO);
// 			is_postpipe = 0;
// 		}
// 		if (is_prepipe)
// 		{
// 			dup2(pipefd[1], STDOUT_FILENO);
// 			close(pipefd[1]);
// 			is_postpipe = 1;
// 			is_prepipe = 0;
// 		}
// 		is_prepipe = handle_pipe(pipe_command[i], pipefd, &is_prepipe);
// 		child[i] = fork();
// 		if (child[i] == 0)
// 		{
// 			execute_child(my_envp, pipe_command + i);
// 		}
// 		free_command(pipe_command[i], -1);
// 	}

int	execute_handler(t_my_envp *my_envp, char **command)
{
	char	***piped_command;
	int		origin_std[2];

	handle_stdfd(origin_std);
	piped_command = get_piped_command(command);
	if (!execute_pipe(my_envp, piped_command))
	{
		*piped_command = handle_redirection(*piped_command);
		if (!exe_bultin(my_envp, *piped_command))
			handle_exe(*piped_command, my_envp);
		free_command(*piped_command, -1);
	}
	handle_stdfd(origin_std);
	return (free(piped_command), 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/28 20:10:16 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_exit_status;

int	execute_command(t_env_var *head, char **command, char **envp)
{
    pid_t	child;
    int		child_status;
    char    *equals_sign;
	char	*full_path;
	char	***piped_command;
	char	***piped_command_ptr;
	int		pipefd[2];
	int		is_prepipe;
	int		origin_stdout;
	int		origin_stdin;

	origin_stdout = dup(STDOUT_FILENO);
	origin_stdin = dup(STDIN_FILENO);

	piped_command = get_piped_command(command);
	piped_command_ptr = piped_command;
	is_prepipe = 0;
	while (*piped_command != NULL)
	{
		if (is_prepipe)
		{
			dup2(pipefd[0], STDIN_FILENO);
			dup2(origin_stdout, STDOUT_FILENO);
		}
		close(pipefd[0]);
		is_prepipe = is_prepipe_command(*piped_command);
		if (is_prepipe)
		{
			if (pipe(pipefd) == -1)
			{
				perror("Pipe failed");
				exit(1);
			}
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		equals_sign = ft_strchr(**piped_command, '=');
		if (equals_sign != NULL)
		{
			*equals_sign = '\0';
			char *name = **piped_command;
			char *value = equals_sign + 1;
			set_env_var(head, name, value);
		}
		else
		{
			child = fork();
			if (child < 0)
			{
				perror("Fork failed");
				exit(1);
			}
			if (child == 0)
			{
				signal(SIGINT, SIG_DFL);
				
				if (!(*piped_command = handle_redirection(*piped_command)))
					exit(0);
				// remove_quotes(command);
				if(!(full_path = get_full_path(*piped_command[0], envp)))
				{
					full_path = ft_strjoin(*piped_command[0], ": command not found\n");
					ft_putstr_fd(full_path, 2);
					free(full_path);
					free_command(*piped_command, -1);
					exit(1);
				}
				if (execve(full_path, *piped_command, envp) == -1)
				{
					perror(*piped_command[0]);
					free_command(*piped_command, -1);
					free(full_path);
					exit(1);
				}
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				waitpid(child, &child_status, WUNTRACED);
				if (WTERMSIG(child_status))
				{
					g_exit_status = child_status >> 8;
					dup2(origin_stdout, STDOUT_FILENO);
					dup2(origin_stdin, STDIN_FILENO);
					while (*piped_command != NULL)
					{
						free_command(*piped_command, -1);
						piped_command++;
					}
					free(piped_command_ptr);
					return (g_exit_status);
				}
			}
		}
		free_command(*piped_command, -1);
		piped_command++;
	}
	free(piped_command_ptr);
	dup2(origin_stdout, STDOUT_FILENO);
	dup2(origin_stdin, STDIN_FILENO);
    return (0);
}
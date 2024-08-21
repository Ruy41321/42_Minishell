/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:29:58 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/21 15:50:45 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long g_exit_status;

char	*get_env_var(char **envp, const char *var_name)
{
    int len = ft_strlen(var_name);
    for (int i = 0; envp[i] != NULL; i++) {
        if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=') {
            return (envp[i] + len + 1);
        }
    }
    return (NULL);
}

int	is_path(char *command) {
	if (ft_strchr(command, '/') != NULL)
		return (1);
	return (0);
}

char	*get_full_path(char *command, char **envp) {
    char	*ret;
	char	*path;
    char	**path_tokens;
	char	*temp_path;
    int		i;
	int		j;

	if (is_path(command))
		return (command);
	ret = NULL;
    i = 0;
    if ((path = get_env_var(envp, "PATH")) == NULL)
		return NULL;
    path = ft_strdup(path);
    path_tokens = ft_split(path, ':');
    free(path);
    while (path_tokens[i] != NULL) {
		temp_path = ft_strjoin(path_tokens[i], "/");
        path = ft_strjoin(temp_path, command);
		free(temp_path);
        if (access(path, F_OK) == 0) {
            ret = path;
			break;
        }
        free(path);
        i++;
    }
	j = 0;
	while(path_tokens[j] != NULL)
	{
		free(path_tokens[j++]);
	}
    free(path_tokens);
    return (ret);
}

int execute_command(t_env_var *head, char **command, char **envp)
{
    pid_t	child;
    int		child_status;
    char    *equals_sign;

	equals_sign = ft_strchr(*command, '=');
	if (equals_sign != NULL)
	{
		*equals_sign = '\0';
		char *name = *command;
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
			if (execve(get_full_path(command[0], envp), command, envp) == -1)
			{
				perror("Error executing command");
				free_command(command);
				exit(1);
			}
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			waitpid(child, &child_status, WUNTRACED);
			if (!WTERMSIG(child_status))
				g_exit_status = child_status >> 8;
		}
	}
    return (0);
}
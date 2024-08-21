/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:32:02 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/21 15:46:58 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_list_command(char *input)
{
	char	**command;
	char	*token;
	int		i;

	i = 0;
	command = malloc(sizeof(char *) * 20);
	if (command == NULL)
	{
		perror("Error allocating memory to command");
		exit(1);
	}
	token = ft_strtok(input, " ");
	while (token != NULL)
	{
		command[i] = token;
		token = ft_strtok(NULL, " ");
		i++;
	}
	command[i] = NULL;
	return (command);
}

void	parse_and_exec(t_env_var *head, char *input, char **envp)
{
    char	**separated_inputs;
	char	**command;
	char	*new_input;
    int		i;

	new_input = substitute_$(head, input);
	separated_inputs = get_separeted_inputs(new_input);
	i = -1;
    while (separated_inputs[++i] != NULL)
	{
		command = get_list_command(separated_inputs[i]);
		execute_command(head, command, envp);
		free_command(command);
	}
    free(separated_inputs);
}

char	*get_pwd(char **envp)
{
	char	*pwd;
	char	*home;
	char	*new_pwd;

	pwd = get_env_var(envp, "PWD");
	home = get_env_var(envp, "HOME");

    // Check if pwd starts with home and replace with ~
    if (home && pwd && strncmp(pwd, home, strlen(home)) == 0) {
        new_pwd = malloc(strlen(pwd) - strlen(home) + 2); // +2 for '~' and null terminator
        if (new_pwd)
		{
            new_pwd[0] = '~';
            strcpy(new_pwd + 1, pwd + strlen(home));
            return (new_pwd);
        }
    }
	return (pwd);
}

char	*get_colored_prompt(char **envp)
{
	char	*pwd;
	char	*user;
	char	*temp;
	char	*temp2;
	char	*prompt;

	pwd = get_pwd(envp);
	user = get_env_var(envp, "USER");
	temp = ft_strjoin(user, "@");
	prompt = ft_strjoin("\033[1;33m", temp);
	free(temp);
	temp = ft_strjoin("\033[0m", ":");
	temp2 = ft_strjoin(prompt, temp);
	free(temp);
	free(prompt);
	temp = ft_strjoin("\033[1;34m", pwd);
	free(pwd);
	prompt = ft_strjoin(temp2, temp);
	free(temp);
	free(temp2);
	temp = ft_strjoin("\033[0m", "$ ");
	temp2 = ft_strjoin(prompt, temp);
	free(temp);
	free(prompt);
	return (temp2);
}

char    *get_input(char **envp)
{
	char    *input;
	char	*prompt;

	prompt = get_colored_prompt(envp);
    input = readline(prompt);
    if (input == NULL)
    {
        free(input);
		free(prompt);
        return(NULL);
    }
    add_history(input);
	free(prompt);
	return (input);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:32:02 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 20:29:40 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_list_command(char *input)
{
	char	**command;
	int		counts[3];
	int		quotes[2];

	init_separeted(&command, quotes, counts, input);
	while (input[++(counts[1])] != '\0')
	{
		if (input[counts[1]] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (input[counts[1]] == '"' && !quotes[0])
			quotes[1] = !quotes[1];
		else if (input[counts[1]] == ' ' && !quotes[0] && !quotes[1])
		{
			command[counts[0]++][counts[2]] = '\0';
			counts[2] = 0;
			command[counts[0]] = safe_malloc(sizeof(char) * \
			(ft_strlen(input) - \
			ft_strlen(command[counts[0] - 1]) + 1));
			continue ;
		}
		command[counts[0]][counts[2]++] = input[counts[1]];
	}
	command[counts[0]][counts[2]] = '\0';
	command[counts[0] + 1] = NULL;
	return (command);
}

void	parse_and_exec(t_my_envp *my_envp, char *input)
{
	char	**separated_inputs;
	char	**command;
	char	*new_input;
	int		i;

	if (only_spaces(input))
		return (free(input));
	new_input = deep_clean_input(input);
	if (!new_input)
		return ;
	separated_inputs = get_separeted_inputs(new_input);
	free(new_input);
	i = 0;
	while (separated_inputs[i] != NULL)
	{
		new_input = substitute_dollars(my_envp, separated_inputs[i++]);
		input = clean_input(new_input, ' ');
		command = get_list_command(input);
		free(input);
		if (execute_command(my_envp, command))
			while (separated_inputs[i] != NULL)
				free(separated_inputs[i++]);
	}
	free(separated_inputs);
}

char	*get_pwd(t_env_var *env)
{
	char	*pwd;
	char	*home;
	char	*new_pwd;

	pwd = get_env_var(env, "PWD");
	home = get_env_var(env, "HOME");
	if (home && pwd && strncmp(pwd, home, strlen(home)) == 0)
	{
		new_pwd = malloc(strlen(pwd) - strlen(home) + 2);
		if (new_pwd)
		{
			new_pwd[0] = '~';
			strcpy(new_pwd + 1, pwd + strlen(home));
			return (new_pwd);
		}
	}
	return (pwd);
}

char	*get_colored_prompt(t_env_var *env)
{
	char	*pwd;
	char	*user;
	char	*temp;
	char	*temp2;
	char	*prompt;

	pwd = get_pwd(env);
	user = get_env_var(env, "USER");
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

char	*get_input(t_my_envp *env)
{
	char	*input;
	char	*prompt;

	prompt = get_colored_prompt(env->exported);
	input = readline(prompt);
	if (input == NULL)
	{
		free(input);
		free(prompt);
		return (NULL);
	}
	add_history(input);
	free(prompt);
	return (input);
}

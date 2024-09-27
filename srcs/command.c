/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:32:02 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/27 15:33:04 by flo-dolc         ###   ########.fr       */
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

void	parse_and_exec(t_my_envp *my_envp, t_parser *pars)
{
	if (only_spaces(pars->input))
		return (free(pars->input));
	pars->new_input = deep_clean_input(pars->input);
	if (!pars->new_input)
		return ;
	pars->new_input = substitute_dollars(my_envp, pars);
	if (only_spaces(pars->new_input))
		return (free(pars->new_input));
	pars->input = clean_input(pars->new_input, ' ');
	pars->command = get_list_command(pars->input);
	free(pars->input);
	execute_handler(my_envp, pars->command);
}

char	*get_pwd(t_env_var *env)
{
	char	*pwd;
	char	*home;
	char	cwd[PATH_MAX];

	pwd = get_env_var(env, "PWD");
	if (!pwd)
		pwd = getcwd(cwd, PATH_MAX);
	home = get_env_var(env, "HOME");
	if (home && pwd && ft_strncmp(pwd, home, ft_strlen(home)) == 0)
		return (ft_strjoin_free("~", ft_strcpy(pwd + ft_strlen(home)), 2));
	return (ft_strdup(pwd));
}

char	*get_colored_prompt(t_env_var *env)
{
	char	*pwd;
	char	*user;
	char	*prompt;

	pwd = get_pwd(env);
	user = get_env_var(env, "USER");
	if (!user)
		user = get_env_var(env, "LOGNAME");
	prompt = ft_strjoin(user, "@");
	prompt = ft_strjoin_free("\033[1;33m", prompt, 2);
	prompt = ft_strjoin_free(prompt, "\033[0m:", 1);
	prompt = ft_strjoin_free(prompt, "\033[1;34m", 1);
	prompt = ft_strjoin_free(prompt, pwd, 3);
	prompt = ft_strjoin_free(prompt, "\033[0m$ ", 1);
	return (prompt);
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

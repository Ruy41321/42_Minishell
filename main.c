/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:05:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/14 17:14:42 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_command(char *input)
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
	token = strtok(input, " ");
	while (token != NULL)
	{
		command[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	command[i] = NULL;
	return (command);
}

void signal_handler1(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		//exit(130);
	}
}

int main()
{
	int		child_status;
    char	*input;
	char	**command;
	pid_t	child;

    while (1)
    {
		signal(SIGINT, signal_handler1);
		signal(SIGQUIT, SIG_IGN);
        input = readline("minishell$ ");
		if (input == NULL)
		{
			free(input);
			exit(0);
		}
		add_history(input);
		command = parse_command(input);

		child = fork();
		if (child < 0) {
            perror("Fork failed");
            exit(1);
        }
		if (child == 0)
		{
			signal(SIGINT, SIG_DFL);
			if (execvp(command[0], command) == -1)
			{
				perror("Error executing command");
				exit(127);
			}
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			waitpid(child, &child_status, WUNTRACED);
		}
		free(input);
		free(command);
    }
    
    return 0;
}
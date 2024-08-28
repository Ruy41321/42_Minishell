/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:53:07 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/28 23:53:28 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**handle_redirection(char **command)
{
	int		i;
	int		len;
	int		c;
	int		status;
	char	**new_command;

	i = -1;
	status = 0;
	c = 0;
	while (command[++i] != NULL)
	{
		len = strlen(command[i]);
		if (ft_strncmp(command[i], "<", len) == 0)
			status = redirect_input(command[i + 1]);
		else if (ft_strncmp(command[i], ">", len) == 0)
			status = redirect_output(command[i + 1], O_WRONLY | O_CREAT | O_TRUNC);
		else if (ft_strncmp(command[i], ">>", len) == 0)
			status = redirect_output(command[i + 1], O_WRONLY | O_CREAT | O_APPEND);
		else
			continue ;
		free(command[i]);
		free(command[i + 1]);
		command[i++] = NULL;
		command[i] = NULL;
		c += 2;
	}
	if (status || command[0] == NULL)
	{
		free_command(command, i);
		return (NULL);
	}
	new_command = (char **)safe_malloc(sizeof(char *) * (i - c + 1));
	new_command[i - c] = NULL;
	while (i-- > 0)
	{
		if (command[i] != NULL)
		{
			new_command[i - c] = ft_strdup(command[i]);
			free(command[i]);
		}
		else
			c--;
	}
	free(command);
	return (new_command);
}

char	*redirect_syntax(char *input)
{
	int		len;
	int		j;
	char	*output;
	char	*token;

	token = syntax_error(input);
	if (token)
	{
		output = ft_strjoin_free(SYNTAX_ERROR, token, 2);
		ft_putstr_fd(output, STDERR_FILENO);
		free(output);
		free(input);
		return (NULL);
	}
	len = ft_strlen(input);
	output = (char *)safe_malloc(len * 3 + 1);
	j = 0;
	for (int i = 0; i < len; i++)
	{
		if (input[i] == '<' || input[i] == '>')
		{
			if (i > 0 && !isspace(input[i - 1]))
				output[j++] = ' ';
			output[j++] = input[i];
			if (input[i] == '>' && input[i + 1] == '>')
			{
				output[j++] = input[++i];
			}
			if (i < len - 1 && !isspace(input[i + 1]))
				output[j++] = ' ';
		}
		else
			output[j++] = input[i];
	}
	output[j] = '\0';
	free(input);
	return (output);
}

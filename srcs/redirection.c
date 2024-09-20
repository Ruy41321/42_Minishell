/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:53:07 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/20 12:32:21 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_redirection(char **command, int *i, int *c)
{
	int	len;
	int	ret;

	len = ft_strlen(command[*i]);
	ret = 0;
	if (ft_strncmp(command[*i], "<", len) == 0)
		ret = redirect_input(command[*i + 1]);
	else if (ft_strncmp(command[*i], ">", len) == 0)
		ret = redirect_output(command[*i + 1], O_WRONLY | O_CREAT | O_TRUNC);
	else if (ft_strcmp(command[*i], ">>") == 0)
		ret = redirect_output(command[*i + 1], O_WRONLY | O_CREAT | O_APPEND);
	else
		return (0);
	free(command[*i]);
	free(command[*i + 1]);
	command[*i] = NULL;
	command[*i + 1] = NULL;
	*i += 1;
	*c += 2;
	return (ret);
}

char	**create_new_command(char **command, int i, int c)
{
	char	**new_command;

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
	return (new_command);
}

void	copy_remaining_chars(char *input, char *output, int *i, int *j)
{
	while (input[*i])
	{
		if (input[*i] == '<' || input[*i] == '>')
		{
			if (*i > 0 && !isspace(input[*i - 1]))
				output[(*j)++] = ' ';
			output[(*j)++] = input[*i];
			if (input[*i] == '>' && input[*i + 1] == '>')
				output[(*j)++] = input[++(*i)];
			if (input[*i] == '<' && input[*i + 1] == '<')
				output[(*j)++] = input[++(*i)];
			if (input[*i + 1] && !isspace(input[*i + 1]))
				output[(*j)++] = ' ';
		}
		else
			output[(*j)++] = input[*i];
		(*i)++;
	}
	output[*j] = '\0';
}

char	**handle_redirection(char **command)
{
	int		i;
	int		c;
	int		status;
	int		len;
	char	**new_command;

	i = -1;
	c = 0;
	status = 0;
	len = ft_arrlen(command);
	while (command[++i] != NULL)
	{
		status = handle_single_redirection(command, &i, &c);
		if (status)
			break ;
	}
	if (status)
		return (free_command(command, len), NULL);
	new_command = create_new_command(command, i, c);
	free(command);
	if (!*new_command)
		return (free_command(new_command, len), NULL);
	return (new_command);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:53:07 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/30 15:02:28 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_redirection(char **command, int *i, int *c)
{
	int	len;
	int	ret;

	len = strlen(command[*i]);
	ret = 0;
	if (ft_strncmp(command[*i], "<", len) == 0)
		ret = redirect_input(command[*i + 1]);
	else if (ft_strncmp(command[*i], ">", len) == 0)
		ret = redirect_output(command[*i + 1], O_WRONLY | O_CREAT | O_TRUNC);
	else if (ft_strncmp(command[*i], ">>", len) == 0)
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
			if (input[*i + 1] && !isspace(input[*i + 1]))
				output[(*j)++] = ' ';
		}
		else
			output[(*j)++] = input[*i];
		(*i)++;
	}
	output[*j] = '\0';
}

char	*redirect_syntax(char *input)
{
	int		len;
	int		i;
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
	i = 0;
	j = 0;
	copy_remaining_chars(input, output, &i, &j);
	free(input);
	return (output);
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
	if (status || command[0] == NULL)
	{
		free_command(command, len);
		return (NULL);
	}
	new_command = create_new_command(command, i, c);
	free(command);
	return (new_command);
}

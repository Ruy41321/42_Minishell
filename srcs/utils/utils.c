/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:19:20 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/09 13:56:04 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	only_spaces(char *input)
{
	while (*input)
	{
		if (*input != ' ')
			return (0);
		input++;
	}
	return (1);
}

void	free_command(char **command, int len)
{
	int	i;

	i = 0;
	if (len == -1)
	{
		while (command[i] != NULL)
		{
			free(command[i]);
			i++;
		}
		free(command);
		return ;
	}
	while (i < len)
	{
		if (command[i] != NULL)
			free(command[i]);
		i++;
	}
	free(command);
}

void	free_command_3d(char ***command)
{
	int	i;

	i = 0;
	while (command[i] != NULL)
	{
		free_command(command[i], -1);
		i++;
	}
	free(command);
}

void	*safe_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
	{
		perror("Error allocating memory");
		exit(1);
	}
	return (ret);
}

void	quotes_check(char *input, int *quotes, int *dquotes, int *i)
{
	if (input[*i] == '\'' && !*dquotes)
		*quotes = !*quotes;
	else if (input[*i] == '"' && !*quotes)
		*dquotes = !*dquotes;
}

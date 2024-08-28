/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:19:20 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/28 17:17:54 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long	g_exit_status;

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

int	len(char **array)
{
	int	count;

	count = 0;
	while (array[count] != NULL)
	{
		count++;
	}
	return (count);
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

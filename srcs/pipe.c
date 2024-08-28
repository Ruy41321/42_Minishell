/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:54:49 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/28 23:15:16 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_prepipe_command(char **command)
{
	int	i;

	i = 0;
	while (command[i] != NULL)
	{
		if (ft_strncmp(command[i], "|", 2) == 0)
		{
			free(command[i]);
			command[i] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	count_pipes(char **command)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (command[i] != NULL)
	{
		if (ft_strncmp(command[i], "|", 2) == 0)
			count++;
		i++;
	}
	return (count);
}

void	split_commands(char ***piped_command, char **command)
{
	int	j;
	int	k;
	int	t;
	int	l;

	j = 0;
	k = 0;
	t = -1;
	l = ft_arrlen_tillstr(command, "|") + 1;
	piped_command[j] = (char **)safe_malloc((l + 1) * sizeof(char *));
	while (command[++t] != NULL)
	{
		piped_command[j][k++] = ft_strdup(command[t]);
		if (ft_strncmp(command[t], "|", 2) == 0)
		{
			piped_command[j++][k] = NULL;
			k = 0;
			command += t + 1;
			t = -1;
			l = ft_arrlen_tillstr(command, "|") + 1;
			piped_command[j] = (char **)safe_malloc((l + 1) * sizeof(char *));
		}
	}
	piped_command[j][k] = NULL;
	piped_command[j + 1] = NULL;
}

char	***get_piped_command(char **command)
{
	int		count;
	char	***piped_command;

	count = count_pipes(command);
	piped_command = (char ***)safe_malloc((count + 1) * sizeof(char **));
	split_commands(piped_command, command);
	free_command(command, -1);
	return (piped_command);
}

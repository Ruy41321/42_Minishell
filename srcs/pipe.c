/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:54:49 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/28 16:45:36 by lpennisi         ###   ########.fr       */
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

char ***get_piped_command(char **command)
{
    int i, j, k, count;
    char ***piped_command;

    count = 1;
    for (i = 0; command[i] != NULL; i++)
    {
        if (ft_strncmp(command[i], "|", 2) == 0)
            count++;
    }

    piped_command = (char ***)malloc((count + 1) * sizeof(char **));
    if (!piped_command)
        return NULL;

    // Itera attraverso l'array di comandi e separa i comandi in base al carattere pipe
    j = 0;
    k = 0;
    piped_command[j] = (char **)malloc((i + 1) * sizeof(char *));
    if (!piped_command[j])
        return NULL;

    for (int t = 0; command[t] != NULL; t++)
    {
        piped_command[j][k++] = ft_strdup(command[t]);
        if (ft_strncmp(command[t], "|", 2) == 0)
        {
            piped_command[j][k] = NULL;
            j++;
            k = 0;
            piped_command[j] = (char **)malloc((i + 1) * sizeof(char *));
            if (!piped_command[j])
                return NULL;
        }
    }
    piped_command[j][k] = NULL;
    piped_command[j + 1] = NULL;
	free_command(command, -1);
    return piped_command;
}
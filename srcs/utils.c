/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:19:20 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/21 16:30:02 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long g_exit_status;

int len(char **array) {
    int count = 0;
    while (array[count] != NULL) {
        count++;
    }
    return count;
}

void	free_command(char **command)
{
	free(*command);
	free(command);
}

char    *get_exit_status(void)
{
    char *exit_status;

    exit_status = ft_itoa(g_exit_status);
    if (exit_status == NULL)
    {
        perror("Error getting exit status");
        exit(1);
    }
    return(exit_status);
}

char    *get_local_var(t_env_var *head, char *name, char *temp)
{
	char	*value;
	int		f;

	f = 0;
	if (temp == NULL)
		value = name;
	else
	{
		value = ft_substr(name, 0, temp - name);
		f = 1;
		temp = NULL;
	}
    while (head->next != NULL)
    {
        if (ft_strcmp(head->name, value) == 0)
            temp = (head->value);
        head = head->next;
    }
    if (head->name != NULL)
	{
        if (ft_strcmp(head->name, value) == 0)
            temp = (head->value);
	}
	if (temp == NULL)
		temp = getenv(value);
	if (f == 1)
		free(value);
    return(temp);
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
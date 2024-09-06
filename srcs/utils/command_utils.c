/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:55 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/06 03:06:54 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_delim(char *input)
{
	const char	*delimiters = " \t\n;|&(){}[]=+-*/!?.,'\"";
	char		*delim;
	char		*temp;
	int			i;

	i = -1;
	delim = NULL;
	while (delimiters[++i] != '\0')
	{
		temp = ft_strchr(input, delimiters[i]);
		if (temp != NULL)
		{
			if (delim == NULL || temp < delim)
				delim = temp;
		}
	}
	return (delim);
}

char	*get_exit_status(void)
{
	char	*exit_status;

	exit_status = ft_itoa(g_exit_status);
	if (exit_status == NULL)
	{
		perror("Error getting exit status");
		exit(1);
	}
	return (exit_status);
}

char	*get_local_var(t_my_envp *my_envp, char *name, char *end)
{
	char	*value;
	char	*result;

	if (end == NULL)
		value = name;
	else
		value = ft_substr(name, 0, end - name);
	result = get_env_var(my_envp->exported, value);
	if (result == NULL)
		result = get_env_var(my_envp->locals, value);
	if (end != NULL)
		free(value);
	return (result);
}

void	init_separeted(char ***separated, int *quotes, int *c, char *input)
{
	c[0] = 0;
	c[1] = -1;
	c[2] = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	*separated = safe_malloc(sizeof(char *) * 20);
	*separated[0] = safe_malloc(sizeof(char) * (ft_strlen(input) + 1));
}

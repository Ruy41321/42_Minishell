/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:02:55 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/28 15:17:44 by lpennisi         ###   ########.fr       */
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
	char	**splitted;

	c[0] = 0;
	c[1] = -1;
	c[2] = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	splitted = ft_split(input, ' ');
	*separated = safe_malloc(sizeof(char *) * (ft_arrlen(splitted) + 1));
	free_command(splitted, -1);
	*separated[0] = safe_malloc(sizeof(char) * (ft_strlen(input) + 1));
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
		g_exit_status = 2;
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

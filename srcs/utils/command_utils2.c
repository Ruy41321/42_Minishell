/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:58:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/30 12:45:16 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*are_consecutives(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == ';' || input[i] == '|' || \
		input[i] == '<' || input[i] == '>')
		{
			i++;
			while (input[i] == ' ')
				i++;
			if (input[i] == ';')
				return (ft_strdup(";`\n"));
			if (input[i] == '|')
				return (ft_strdup("|`\n"));
		}
		else
			i++;
	}
	return (NULL);
}

char	*get_spaced_input(char *input, int len, int new_len)
{
	int		j;
	char	*result;
	int		i;

	j = 0;
	i = -1;
	result = safe_malloc(new_len + 1);
	while (++i < len)
	{
		if (input[i] == '|')
		{
			result[j++] = ' ';
			result[j++] = '|';
			result[j++] = ' ';
		}
		else
			result[j++] = input[i];
	}
	result[j] = '\0';
	free(input);
	return (result);
}

char	*pipe_syntax(char *input)
{
	int		len;
	int		new_len;
	char	*result;
	int		i;

	len = ft_strlen(input);
	new_len = len;
	if (input[0] == '|' || input[ft_strlen(input) - 1] == '|')
		return (NULL);
	i = -1;
	while (++i < len)
	{
		if (input[i] == '|')
			new_len += 2;
	}
	result = get_spaced_input(input, len, new_len);
	return (result);
}

char	*deep_clean_input(char *input)
{
	char	*new_input;
	char	*temp;

	new_input = clean_input(input, ' ');
	if (!new_input)
		return (NULL);
	new_input = clean_input(new_input, ';');
	if (!new_input)
		return (NULL);
	new_input = redirect_syntax(new_input);
	if (!new_input)
		return (NULL);
	new_input = pipe_syntax(new_input);
	if (!new_input)
		return (NULL);
	temp = are_consecutives(new_input);
	if (temp)
	{
		free(new_input);
		new_input = ft_strjoin_free(SYNTAX_ERROR, temp, 2);
		ft_putstr_fd(new_input, STDERR_FILENO);
		free(new_input);
		return (NULL);
	}
	return (new_input);
}
// prova ad eseguire dovrebbe andare , da controllare <> e | con le apici
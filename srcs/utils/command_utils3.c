/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:41:49 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/30 17:44:49 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *input)
{
	int		i;
	int		j;
	int		len;
	char	*temp;
	int		quotes[2];

	i = -1;
	j = 0;
	len = strlen(input);
	temp = safe_malloc(len + 1);
	quotes[0] = 0;
	quotes[1] = 0;
	while (++i < len)
	{
		if (input[i] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (input[i] == '"' && !quotes[0])
			quotes[1] = !quotes[1];
		else
			temp[j++] = input[i];
	}
	temp[j] = '\0';
	return (temp);
}

void	remove_quotes_2d(char **command)
{
	char	*temp;

	while (*command)
	{
		temp = remove_quotes(*command);
		free(*command);
		*command = temp;
		(command)++;
	}
}

char	*remove_consecutive_chars(char *input, char ch)
{
	int		i;
	int		j;
	int		len;
	char	*temp;
	int		quotes[2];

	i = -1;
	j = 0;
	len = strlen(input);
	temp = safe_malloc(len + 1);
	quotes[0] = 0;
	quotes[1] = 0;
	while (++i < len)
	{
		quotes_check(input, quotes, quotes + 1, &i);
		if (!(input[i] == ch && input[i + 1] == ch) || quotes[0] || quotes[1])
			temp[j++] = input[i];
	}
	temp[j] = '\0';
	return (temp);
}

char	*trim_edges(char *input, char ch)
{
	char	*start;
	char	*end;

	start = input;
	while (*start == ch)
		start++;
	end = input + strlen(input) - 1;
	while (end > start && *end == ch)
		end--;
	*(end + 1) = '\0';
	start = ft_strdup(start);
	free(input);
	return (start);
}

char	*clean_input(char *input, char ch)
{
	char	*temp;
	char	*result;

	if (ch == ';')
	{
		if (input[strlen(input) - 1] == ';')
			input[ft_strlen(input) - 1] = '\0';
		temp = remove_consecutive_chars(input, ch);
		result = trim_edges(temp, ch);
		if (ft_strncmp(input, result, ft_strlen(input)) != 0)
		{
			free(result);
			free(input);
			printf("minishell: syntax error near unexpected token `;'\n");
			return (NULL);
		}
		temp = clean_input(result, ' ');
		free(input);
		return (temp);
	}
	temp = remove_consecutive_chars(input, ch);
	result = trim_edges(temp, ch);
	free(input);
	return (result);
}

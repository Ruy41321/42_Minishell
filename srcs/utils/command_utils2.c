/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:58:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/24 13:14:29 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*are_consecutives(char *input)
{
	int	i;
	int	quotes[2];

	i = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	while (input[i] != '\0')
	{
		quotes_check(input, quotes, quotes + 1, &i);
		if ((input[i] == ';' || input[i] == '|' || \
		input[i] == '<' || input[i] == '>') && !quotes[0] && !quotes[1])
		{
			i++;
			while (input[i] == ' ')
				i++;
			if (input[i] == '|')
				return (ft_strdup("|'\n"));
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
	int		quotes[2];

	j = 0;
	i = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	result = safe_malloc(new_len + 1);
	while (++i < len)
	{
		quotes_check(input, quotes, quotes + 1, &i);
		if (input[i] == '|' && !quotes[0] && !quotes[1])
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
	int		quotes[2];

	len = ft_strlen(input);
	new_len = len;
	quotes[0] = 0;
	quotes[1] = 0;
	if (input[0] == '|' || input[ft_strlen(input) - 1] == '|')
		return (free(input), NULL);
	i = -1;
	while (++i < len)
	{
		if (input[i] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (input[i] == '"' && !quotes[0])
			quotes[1] = !quotes[1];
		else if (input[i] == '|' && !quotes[0] && !quotes[1])
			new_len += 2;
	}
	result = get_spaced_input(input, len, new_len);
	return (result);
}

void	print_syntax_error(char *input)
{
	g_exit_status = 2;
	ft_putstr_fd(SYNTAX_ERROR, STDERR_FILENO);
	ft_putstr_fd(input, STDERR_FILENO);
}

char	*deep_clean_input(char *input)
{
	char	*new_input;
	char	*temp;

	new_input = clean_input(input, ' ');
	if (!new_input)
		return (NULL);
	if (ft_strlen(new_input) == 0)
		return (free(new_input), NULL);
	new_input = pipe_syntax(new_input);
	if (!new_input)
		return (print_syntax_error("|'\n"), NULL);
	new_input = redirect_syntax(new_input);
	if (!new_input)
		return (NULL);
	temp = are_consecutives(new_input);
	if (temp)
	{
		free(new_input);
		print_syntax_error(temp);
		return (free(temp), NULL);
	}
	return (new_input);
}

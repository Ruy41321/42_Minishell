/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:08:34 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/27 15:06:25 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_separeted_inputs(char *input)
{
	char	**separated_inputs;
	int		counts[3];
	int		quotes[2];

	init_separeted(&separated_inputs, quotes, counts, input);
	while (input[++(counts[1])] != '\0')
	{
		if (input[counts[1]] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (input[counts[1]] == '"' && !quotes[0])
			quotes[1] = !quotes[1];
		else if (input[counts[1]] == ';' && !quotes[0] && !quotes[1])
		{
			separated_inputs[counts[0]++][counts[2]] = '\0';
			counts[2] = 0;
			separated_inputs[counts[0]] = safe_malloc(sizeof(char) * \
			(ft_strlen(input) - \
			ft_strlen(separated_inputs[counts[0] - 1]) + 1));
			continue ;
		}
		separated_inputs[counts[0]][counts[2]++] = input[counts[1]];
	}
	separated_inputs[counts[0]][counts[2]] = '\0';
	separated_inputs[counts[0] + 1] = NULL;
	return (separated_inputs);
}

void	handle_expansion(char *input, int *i, char **new_input, t_my_envp *env)
{
	char	*delim;

	delim = get_delim(input + *i + 1);
	*new_input = ft_strjoin_free(*new_input, \
	get_local_var(env, input + *i + 1, delim), 1);
	if (delim != NULL)
		(*i) += (int)(delim - (input + *i + 1));
	else
		(*i) += ft_strlen(input + *i + 1);
}

int	is_val_start(char c)
{
	if (c == '_' || ft_isalnum(c))
		return (1);
	return (0);
}

/**
 * quotes[2];
 * quotes[0] : single quotes flag
 * quotes[1] : double quotes flag
 */
char	*fill_new_input(t_my_envp *my_envp, char *input, int *i, int *quotes)
{
	char	temp[2];
	char	*new_input;

	new_input = ft_strdup("");
	if (input[*i] == '$' && input[*i + 1] != '\0' && \
	quotes[0] == 0 && (is_val_start(input[*i + 1]) || input[*i + 1] == '?'))
	{
		if (input[*i + 1] == '?')
		{
			new_input = ft_strjoin_free(new_input, get_exit_status(), 3);
			(*i)++;
		}
		else
			handle_expansion(input, i, &new_input, my_envp);
		g_exit_status = 0;
	}
	else
	{
		temp[0] = input[*i];
		temp[1] = '\0';
		new_input = ft_strjoin_free(new_input, temp, 1);
	}
	return (new_input);
}

char	*substitute_dollars(t_my_envp *my_envp, char *input)
{
	int		i;
	char	*new_input;
	int		quotes[2];

	i = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	new_input = ft_strdup("");
	while (input[++i] != '\0')
	{
		if (input[i] == '\'' && quotes[1] == 0)
			quotes[0] = !quotes[0];
		else if (input[i] == '\"' && quotes[0] == 0)
			quotes[1] = !quotes[1];
		new_input = ft_strjoin_free(new_input, \
		fill_new_input(my_envp, input, &i, quotes), 3);
	}
	if (quotes[0] == 1 || quotes[1] == 1)
	{
		free(new_input);
		new_input = ft_strdup("echo Error: unmatched quotes");
	}
	free(input);
	return (new_input);
}

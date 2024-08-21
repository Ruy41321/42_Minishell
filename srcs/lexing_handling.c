/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:08:34 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/21 16:48:32 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern long long g_exit_status;

char	**get_separeted_inputs(char *input)
{
	char	**separated_inputs;
	int		i;

	separated_inputs = malloc(sizeof(char *) * 20);
	if (separated_inputs == NULL)
	{
		perror("Error allocating memory to separated inputs");
		exit(1);
	}
	i = 0;
	separated_inputs[i] = ft_strtok(input, ";");
	while (separated_inputs[i++] != NULL)
	{
		separated_inputs[i] = ft_strtok(NULL, ";");
	}
	return (separated_inputs);
}

void handle_variable_expansion(char *input, int *i, char **new_input, t_env_var *head) {
    char *space;

	space = ft_strchr(input + *i + 1, ' ');
    *new_input = ft_strjoin_free(*new_input, get_local_var(head, input + *i + 1, space), 1);
    if (space != NULL) {
        (*i) += (int)(space - (input + *i + 1));
    } else {
        (*i) += ft_strlen(input + *i + 1);
    }
}

void	fill_new_input(char **new_input, char *input, int *i, t_env_var *head)
{
	char	temp[2];
	
	if (input[*i] == '$' && input[*i+1] != '\0')
	{
		if (input[*i+1] == '?')
		{
			*new_input = ft_strjoin_free(*new_input, get_exit_status(), 3);
			(*i)++;
		}
		else
			handle_variable_expansion(input, i, new_input, head);       
	}
	else
	{
		temp[0] = input[*i];
		temp[1] = '\0';
		*new_input = ft_strjoin_free(*new_input, temp, 1);
	}	
}

char    *substitute_$(t_env_var *head, char *input)
{
	int		i;
    char	*new_input;

	i = -1;
	new_input = ft_strdup("");
	while (input[++i] != '\0')
		fill_new_input(&new_input, input, &i, head);
	free(input);
    return (new_input);
}

void set_env_var(t_env_var *head, char *name, char *value) 
{
    t_env_var   *new;

    if (head->name == NULL)
    {
        head->name = ft_strdup(name);
        head->value = ft_strdup(value);
        return;
    }
    while (head->next != NULL)
    {
        if (ft_strcmp(head->name, name) == 0)
        {
            free(head->value);
            head->value = ft_strdup(value);
            return;
        }
        head = head->next;
    }
    if (ft_strcmp(head->name, name) == 0)
        {
            free(head->value);
            head->value = ft_strdup(value);
            return;
        }
    new = malloc(sizeof(t_env_var));
    if (new == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->name = ft_strdup(name);
    new->value = ft_strdup(value);
    new->next = NULL;
    head->next = new;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:53:07 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/24 21:52:36 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redirect_char(char c) {
    return c == '<' || c == '>';
}

int	redirect_input(char *file)
{
	int fd;
	char *str;

	if (is_redirect_char(file[0]))
	{
		if (file[0] == '<')
			str = ft_strdup("minishell: syntax error near unexpected token `<`\n");
		else
			str = ft_strdup("minishell: syntax error near unexpected token `>`\n");
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
		return (1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		str = ft_strjoin("minishell: ", file);
		perror(str);
		free(str);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redirect_output(char *file, int flags)
{
	int fd;
	char *str;

	if (is_redirect_char(file[0]))
	{
		if (file[0] == '<')
			str = ft_strdup("minishell: syntax error near unexpected token `<`\n");
		else
			str = ft_strdup("minishell: syntax error near unexpected token `>`\n");
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
		return (1);
	}
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		str = ft_strjoin("minishell: ", file);
		perror(str);
		free(str);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

//da far funzionare redirect consecutivi

char	**handle_redirection(char **command)
{
	int i;
	int len;
	int c;
	int status;
	char **new_command;

	i = -1;
	status = 0;
	c = 0;
	while (command[++i] != NULL)
	{
		len = strlen(command[i]);
		if (ft_strncmp(command[i], "<", len) == 0)
			status = redirect_input(command[i + 1]);
		else if (ft_strncmp(command[i], ">", len) == 0)
			status = redirect_output(command[i + 1], O_WRONLY | O_CREAT | O_TRUNC);
		else if (ft_strncmp(command[i], ">>", len) == 0)
			status = redirect_output(command[i + 1], O_WRONLY | O_CREAT | O_APPEND);
		else
			continue;
		free(command[i]);
		free(command[i + 1]);
		command[i++] = NULL;
		command[i] = NULL;
		c += 2;
	}
	if (status || command[0] == NULL)
	{
		free_command(command, i);
		return (NULL);
	}
	new_command = (char **)safe_malloc(sizeof(char *) * (i - c + 1));
	new_command[i - c] = NULL;
	while (i-- > 0)
	{
		if (command[i] != NULL)
		{
			new_command[i - c] = ft_strdup(command[i]);
			free(command[i]);
		}
		else
			c--;
	}
	free(command);
	return (new_command);
}

char *syntax_error(char *input) {
    int len = strlen(input);
    int i = 0;
    while (i < len) {
        if (input[i] == '<' || input[i] == '>') {
            if (i == len - 1 || input[i + 1] == '\n')
                return strdup("newline");
			if (input[i] == '<')
				if (is_redirect_char(input[i + 1]))
					return (ft_strdup("<"));

			if (input[i] == '>')
			{
				if (input[i + 1] == '>' && is_redirect_char(input[i + 2]))
					return (ft_strdup(">"));
				if (input[i + 1] == '<')
					return (ft_strdup(">"));
			}
        }
        i++;
    }
    return NULL;
}

char *redirect_syntax(char *input)
{
    int len;
	int j;
	char *output;
	char *token;

    if ((token = syntax_error(input)))
	{
		output = ft_strjoin("minishell: syntax error near unexpected token `", token);
		free(token);
		token = ft_strjoin(output, "`\n");
		free(output);
		ft_putstr_fd(token, STDERR_FILENO);
		free(token);
		free(input);
		return NULL;
	}
	len = ft_strlen(input);
    output = (char *)safe_malloc(len * 3 + 1);
    j = 0;
    for (int i = 0; i < len; i++)
    {
        if (input[i] == '<' || input[i] == '>')
        {
            if (i > 0 && !isspace(input[i - 1]))
                output[j++] = ' ';
            output[j++] = input[i];
            if (input[i] == '>' && input[i + 1] == '>')
            {
                output[j++] = input[++i];
            }
            if (i < len - 1 && !isspace(input[i + 1]))
                output[j++] = ' ';
        }
        else
            output[j++] = input[i];
    }
    output[j] = '\0';
    free(input);
    return output;
}
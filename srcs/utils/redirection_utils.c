/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:45:19 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/24 15:21:39 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect_char(char c)
{
	return (c == '<' || c == '>');
}

int	redirect_input(char *fil)
{
	int		fd;
	char	*str;
	char	*file;

	file = remove_quotes(fil);
	if (is_redirect_char(file[0]))
	{
		if (file[0] == '<')
			str = ft_strjoin_free(SYNTAX_ERROR, "<`\n", 0);
		else
			str = ft_strjoin_free(SYNTAX_ERROR, ">`\n", 0);
		ft_putstr_fd(str, STDERR_FILENO);
		free(file);
		return (free(str), 1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		str = ft_strjoin("minishell: ", file);
		perror(str);
		free(file);
		return (free(str), 1);
	}
	dup2(fd, STDIN_FILENO);
	free(file);
	return (close(fd), 0);
}

int	redirect_output(char *fil, int flags)
{
	int		fd;
	char	*str;
	char	*file;

	file = remove_quotes(fil);
	if (is_redirect_char(file[0]))
	{
		if (file[0] == '<')
			str = ft_strjoin_free(SYNTAX_ERROR, "<`\n", 0);
		else
			str = ft_strjoin_free(SYNTAX_ERROR, ">`\n", 0);
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
		return (free(file), 1);
	}
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		str = ft_strjoin("minishell: ", file);
		perror(str);
		free(str);
		return (free(file), 1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (free(file), 0);
}

char	*redirect_error(char *input, int i, int len)
{
	if (i == len - 1 || input[i + 1] == '\n')
		return (ft_strdup("newline'\n"));
	if (input[i] == '<')
	{
		if ((input[i + 1] == '<' && is_redirect_char(input[i + 2])) \
		|| input[i + 1] == '>')
			return (ft_strdup("<'\n"));
	}
	else if (input[i] == '>')
	{
		if ((input[i + 1] == '>' && is_redirect_char(input[i + 2])) \
		|| input[i + 1] == '<')
			return (ft_strdup(">'\n"));
	}
	
	return (NULL);
}

char	*syntax_error(char *input)
{
	int		len;
	int		i;
	int		quotes[2];
	char	*error;

	i = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	len = ft_strlen(input);
	while (++i < len)
	{
		quotes_check(input, quotes, quotes + 1, &i);
		if ((input[i] == '<' || input[i] == '>') && !quotes[0] && !quotes[1])
		{
			error = redirect_error(input, i, len);
			if (error)
				return (error);
		}
	}
	return (NULL);
}

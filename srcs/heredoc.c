/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:49 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/21 04:30:14 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc_fd(int count)
{
	char	*filename;
	int		fd;
	char	*str;

	filename = ft_strjoin_free("/tmp/heredoc", ft_itoa(count), 2);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		str = ft_strjoin_free("minishell: ", filename, 2);
		perror(str);
		return (free(str), 1);
	}
	free(filename);
	return (fd);
}

void	print_heredoc_warning(char *terminator)
{
	ft_putstr_fd("minishell: warning: ", STDERR_FILENO);
	ft_putstr_fd(HEREDOC_WARN, STDERR_FILENO);
	ft_putstr_fd(terminator, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

int	redirect_input_heredoc(char *terminator, int count)
{
	int		fd;
	char	*line;

	fd = get_heredoc_fd(count);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(terminator);
			break ;
		}
		if (ft_strncmp(line, terminator, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	return (0);
}

char	**substitute_heredoc(char **command)
{
	char	**new_command;
	char	*filename;
	int		i;
	int		count;
	int		len;

	len = ft_arrlen(command);
	count = 0;
	i = -1;
	new_command = (char **)safe_malloc(sizeof(char *) * (len + 1));
	new_command[len] = NULL;
	while (command[++i] != NULL)
	{
		if (ft_strcmp(command[i], "<<") == 0)
		{
			filename = ft_strjoin_free("/tmp/heredoc", ft_itoa(count++), 2);
			new_command[i] = ft_strdup("<");
			new_command[++i] = filename;
		}
		else
			new_command[i] = ft_strdup(command[i]);
	}
	return (free_command(command, len), new_command);
}

char	**handle_heredoc(char **command)
{
	char	**new_command;
	int		i;
	int		count;

	i = -1;
	count = 0;
	while (command[++i] != NULL)
	{
		if (ft_strcmp(command[i], "<<") == 0)
			if (redirect_input_heredoc(command[++i], count++))
				return (free_command(command, -1), NULL);
	}
	new_command = substitute_heredoc(command);
	return (new_command);
}
